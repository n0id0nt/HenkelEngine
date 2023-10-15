#include "Window.h"

#include "Camera.h"
#include "opengl\openglHelper.h"
#include "Input.h"
#include <imgui_internal.h>

Window::Window(std::string name, int x, int y, int w, int h)
	: m_running(true), Width(w), Height(h), m_clearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f))
{
	// add callbacks
	Input::window = this;

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	m_window = SDL_CreateWindow("Rubik's Cube", x, y, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	m_context = SDL_GL_CreateContext(m_window);

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

	m_scene = std::make_unique<Scene>(this, "../Example/", "TestLevel.tmx");

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;		// Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	io.ConfigDockingWithShift = false;

	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(m_window, m_context);
	ImGui_ImplOpenGL3_Init();
}

Window::~Window()
{
	// Exit
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(m_context);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void Window::Input()
{
	// input
	Input::Update();

	// quit
	if (Input::Quit())
		OnClose();
}

void Window::Loop(float deltaTime)
{
	Input();

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	// Update
	m_scene->Update(deltaTime);

	// Define the viewport dimensions
	glViewport(0, 0, Width, Height);
	// Clear the colorbuffer
	GLCall(glClearColor(m_clearColor.x * m_clearColor.w, m_clearColor.y * m_clearColor.w, m_clearColor.z * m_clearColor.w, m_clearColor.w));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	// Render
	m_scene->Render();

	// Render ImGui
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_GL_SwapWindow(m_window);
}

void Window::OnClose()
{
	m_running = false;
}

void Window::OnResize(int w, int h)
{
	Width = w;
	Height = h;
}
