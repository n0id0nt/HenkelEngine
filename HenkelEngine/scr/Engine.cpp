#include "Engine.h"
#include "opengl\openglHelper.h"
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include "opengl\DebugRenderer.h"
#include <imgui_impl_opengl3.h>

Engine::Engine() : m_clearColor({ 0.2f, 0.3f, 0.3f, 1.0f })
{
	InitEngine();
}

Engine::~Engine()
{
	// Exit
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	DebugRenderer::ExitDebugRenderer();

	SDL_Quit();
}

void Engine::InitEngine()
{
	m_input = std::make_unique<Input>();

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	m_window = std::make_unique<Window>("Henkel Engine", 150, 150, 640, 420, true, false);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;

	// Initialize GLEW to setup the OpenGL Function pointers
	ASSERT(GLEW_OK == glewInit());

	std::cout << glGetString(GL_VERSION) << std::endl;

	// Setup some OpenGL options
	GLCall(glEnable(GL_DEPTH_TEST));

	// enable alpha support
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;		// Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigDockingWithShift = false;

	ImGui::StyleColorsDark();
	m_window->SetUpImGuiForWindow();
	ImGui_ImplOpenGL3_Init();

	DebugRenderer::InitDebugRenderer();

	m_scene = std::make_unique<Scene>(this, "../Example/", "TestLevel.tmx");
}

void Engine::Loop()
{
	float deltaTime = 0.f;
	while (m_window->IsRunning())
	{
		m_frameStart = SDL_GetTicks();

		m_input->Update();
		if (m_input->WasWindowResized())
		{
			m_window->Resize(m_input->WindowSize());
			m_input->ClearWindowResizedFlag();
		}
		if (m_input->Quit())
		{
			m_window->OnClose();
		}

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		// Update
		m_scene->Update(deltaTime);

		// Define the viewport dimensions
		glViewport(0, 0, m_window->GetWidth(), m_window->GetHeight());
		// Clear the colorbuffer
		GLCall(glClearColor(m_clearColor.x * m_clearColor.w, m_clearColor.y * m_clearColor.w, m_clearColor.z * m_clearColor.w, m_clearColor.w));
		//GLCall(glClearColor(1.f, 0.8f, 0.2f, 1.f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		// Render
		m_scene->Render();

		// Render ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		m_window->SwapBuffers();

		m_frameTime = SDL_GetTicks() - m_frameStart;
		if (FRAME_DELAY > m_frameTime)
		{
			SDL_Delay(FRAME_DELAY - m_frameTime);
		}
		deltaTime = (SDL_GetTicks() - m_frameStart) / 1000.f;
	}
}

Window* Engine::GetWindow() const
{
	return m_window.get();
}

Scene* Engine::GetCurrentScene() const
{
	return m_scene.get();
}

Input* Engine::GetInput() const
{
	return m_input.get();
}
