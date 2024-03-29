#include "Engine.h"
#include "opengl/openglHelper.h"
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include "opengl/DebugRenderer.h"
#include "imgui_impl_opengl3.h"

Engine*Engine::s_engine = nullptr;

const std::string s_testMap1 = "TestLevel.tmx";
const std::string s_testMap2 = "AutoMappingTestLevel.tmx";

Engine::Engine() 
{
}

Engine* Engine::GetInstance()
{
 	if (!s_engine) 
		s_engine = new Engine();

	return s_engine;
}

Engine::~Engine()
{
}

void Engine::InitEngine()
{
	m_clearColor = { 0.2f, 0.3f, 0.3f, 1.0f };
	m_projectDirectory = "../Example/";

	m_input = std::make_unique<Input>();
	m_time = std::make_unique<Time>();

	m_input->LoadInputBindings(m_projectDirectory, "Settings/InputBindings/InputBindings.xml");

	m_resourcePool = std::make_unique<ResourcePool>();
	SDL_Init(SDL_INIT_EVERYTHING);


	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	m_window = std::make_unique<Window>("Henkel Engine", 150, 150, 640, 420, true, false);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;

	// Initialize GLEW to setup the OpenGL Function pointers
	ASSERT(GLEW_OK == glewInit());

	std::cout << glGetString(GL_VERSION) << std::endl;

	// Setup some OpenGL options
	//GLCall(glEnable(GL_DEPTH_TEST));

	// enable alpha support
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;		// Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigDockingWithShift = false;

	ImGui::StyleColorsDark();
	SetDarkThemeColors();
	m_window->SetUpImGuiForWindow();
	ImGui_ImplOpenGL3_Init();

	DebugRenderer::InitDebugRenderer();

	m_scene = std::make_unique<Scene>(m_projectDirectory, s_testMap2);
}

void Engine::ExitEngine()
{
	DebugRenderer::ExitDebugRenderer();
	m_resourcePool.reset();
	m_input.reset();
	m_scene.reset();
	m_window.reset();
	m_time.reset();

	// Exit
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_Quit();

	delete s_engine;
	s_engine = nullptr;
}

void Engine::Loop()
{
	while (m_window->IsRunning())
	{
		m_frameStart = SDL_GetTicks();
		m_time->SetTime(m_frameStart);

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
		m_scene->Update();

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
		m_time->SetDeltaTime((SDL_GetTicks() - m_frameStart) / 1000.f);

		float delayedFrameTime = SDL_GetTicks() - m_frameStart;
		float fps = (delayedFrameTime > 0) ? 1000.0f / delayedFrameTime : 0.0f;
		m_window->SetWindowName("Henkel Engine - fps: " + std::to_string(fps));
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

Time* Engine::GetTime() const
{
	return m_time.get();
}

ResourcePool* Engine::GetResourcePool() const
{
	return m_resourcePool.get();
}

std::string Engine::GetProjectDirectory() const
{
	return m_projectDirectory;
}

void Engine::SetDarkThemeColors()
{
	auto primaryColor = ImVec4{ 0.1f, 0.505f, 0.11f, 1.0f };
	auto primaryColorHighlight = ImVec4{ 0.15f, 0.605f, 0.18f, 1.0f };
	auto secondaryColor = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };
	auto bgColor = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

	auto& colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = bgColor;

	// Headers
	colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Buttons
	colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Frame BG
	colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Tabs
	colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
	colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
	colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

	// Title
	colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Slider
	colors[ImGuiCol_SliderGrab] = primaryColor;
	colors[ImGuiCol_SliderGrabActive] = primaryColorHighlight;

	// Check Mark
	colors[ImGuiCol_CheckMark] = primaryColorHighlight;

}

unsigned int Engine::GetCollisionLayer(std::string layerName)
{
	return m_collisionLayers.GetLayer(layerName);
}
