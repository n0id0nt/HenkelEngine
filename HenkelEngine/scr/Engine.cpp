#include "Engine.h"
#include "opengl/openglHelper.h"
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <format>
#include "opengl/DebugRenderer.h"
#include "imgui_impl_opengl3.h"
#include "HelperFunctions.h"
#include "FileParser.h"
#include "ECS\Component\TransformComponent.h"
#include "ECS\Component\PhysicsBodyComponents\CollisionBodyComponent.h"
#include "ECS\Component\PhysicsBodyComponents\PhysicsBodyComponent.h"
#include "ECS\Component\PhysicsBodyComponents\StaticBodyComponent.h"
#include "ECS\Component\PhysicsBodyComponents\TileMapCollisionBodyComponent.h"
#include "ECS/Component/ScriptComponent.h"
#include "ECS/Component/RenderComponents/SpriteComponent.h"
#include "ECS/Component/RenderComponents/RenderComponent.h"
#include <ECS\Component\SpriteAnimationComponent.h>
#include <ECS\Component\CameraComponent.h>
#include <ECS\Component\UIComponent.h>
#include <ECS\Component\LevelComponent.h>
#include "UI\UIArea.h"
#include "UI\UIText.h"

Engine*Engine::s_engine = nullptr;

const std::string s_world = "FirstTestWorld.world";

Engine::Engine()
{
}

void Engine::InitialiseLua()
{
	m_lua = sol::state();
	m_lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math);
	// Change the working directory
	DEBUG_PRINT(Engine::GetInstance()->GetProjectDirectory());
	m_lua.script(std::format("package.path = '{}Scripts/?.lua'", Engine::GetInstance()->GetProjectDirectory()));

	m_input->LUABind(m_lua);
	m_time->LUABind(m_lua);
	m_messenger->LUABind(m_lua);

	HenkelEngine::LUABindLibraries(m_lua);

	Entity::LUABind(m_lua);

	TransformComponent::LUABind(m_lua);
	CollisionBodyComponent::LUABind(m_lua);
	PhysicsBodyComponent::LUABind(m_lua);
	TileMapCollisionBodyComponent::LUABind(m_lua);
	StaticBodyComponent::LUABind(m_lua);
	ScriptComponent::LUABind(m_lua);
	SpriteComponent::LUABind(m_lua);
	SpriteAnimationComponent::LUABind(m_lua);
	CameraComponent::LUABind(m_lua);
	UIComponent::LUABind(m_lua);
	RenderComponent::LUABind(m_lua);
	LevelComponent::LUABind(m_lua);

	UIArea::LUABind(m_lua);
	UIText::LUABind(m_lua);
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
	m_messenger = std::make_unique<Messenger>();

	m_input->LoadInputBindings(m_projectDirectory, "Settings/InputBindings/InputBindings.xml");

	m_resourcePool = std::make_unique<ResourcePool>();
	SDL_Init(SDL_INIT_EVERYTHING);


	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	m_window = std::make_unique<Window>("Henkel Engine", 150, 150, 640, 420, true, false);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;

	// Initialize GLEW to setup the OpenGL Function pointers
	ASSERT(GLEW_OK == glewInit());

	DEBUG_PRINT(reinterpret_cast<const char*>(glGetString(GL_VERSION)));

	// Setup some OpenGL options
	//GLCall(glEnable(GL_DEPTH_TEST));

	// enable alpha support
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

#ifdef _DEBUG
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;		// Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigDockingWithShift = false;

	ImGui::StyleColorsDark();
	HenkelEngine::SetDarkThemeColors();
	m_window->SetUpImGuiForWindow();
	ImGui_ImplOpenGL3_Init();

	DebugRenderer::InitDebugRenderer();
#endif //_DEBUG

	m_world = std::make_unique<World>();

	InitialiseLua();

	m_world->LUABind(m_lua);
	FileParser::LoadWorld(m_world.get(), m_projectDirectory, s_world);
}

void Engine::ExitEngine()
{
	DebugRenderer::ExitDebugRenderer();
	m_input.reset();
	m_world.reset();
	m_window.reset();
	m_time.reset();
	m_resourcePool.reset();

#ifdef _DEBUG
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
#endif // _DEBUG

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

#ifdef _DEBUG
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
#endif // _DEBUG

		// Update
		m_world->Update();

		// Define the viewport dimensions
		glViewport(0, 0, m_window->GetWidth(), m_window->GetHeight());
		// Clear the colorbuffer
		GLCall(glClearColor(m_clearColor.x * m_clearColor.w, m_clearColor.y * m_clearColor.w, m_clearColor.z * m_clearColor.w, m_clearColor.w));
		//GLCall(glClearColor(1.f, 0.8f, 0.2f, 1.f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		// Render
		m_world->Render();

#ifdef _DEBUG
		// Render ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif // _DEBUG

		m_window->SwapBuffers();

		m_frameTime = SDL_GetTicks() - m_frameStart;
		if (FRAME_DELAY > m_frameTime)
		{
			SDL_Delay(FRAME_DELAY - m_frameTime);
		}
		m_time->SetDeltaTime((SDL_GetTicks() - m_frameStart) / 1000.f);

#ifdef _DEBUG
		float delayedFrameTime = SDL_GetTicks() - m_frameStart;
		float fps = (delayedFrameTime > 0) ? 1000.0f / delayedFrameTime : 0.0f;

		m_fpsHistory[m_currentFPSIndex++] = fps;
		m_currentFPSIndex %= FPS;

		float minFPS = fps;
		float sumFPS = 0.0f;
		for (float curFPS : m_fpsHistory)
		{
			minFPS = glm::min(minFPS, curFPS);
			sumFPS += curFPS;
		}

		m_window->SetWindowName(std::format("Henkel Engine - fps: {} - min fps: {} - avg fps: {}", (int)fps, (int)minFPS, (int)sumFPS/(int)FPS));
#endif //_DEBUG
	}
}

Window* Engine::GetWindow() const
{
	return m_window.get();
}

World* Engine::GetWorld() const
{
	return m_world.get();
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

Messenger* Engine::GetMessenger() const
{
	return m_messenger.get();
}

std::string Engine::GetProjectDirectory() const
{
	return m_projectDirectory;
}

unsigned int Engine::GetCollisionLayer(std::string layerName)
{
	return m_collisionLayers.GetLayer(layerName);
}

sol::state& Engine::GetSolState()
{
	return m_lua;
}
