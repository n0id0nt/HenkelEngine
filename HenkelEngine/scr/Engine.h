#pragma once

#include <memory>
#include "Window.h"
#include <SDL2\SDL.h>
#include <string>
#include "TimeHelper.h"
#include "glm\glm.hpp"
#include "Input.h"
#include "ResourcePool.h"
#include "Messenger.h"
#include "Physics\CollisionLayers.h"
#include "World.h"
const unsigned int FPS = 60;
const unsigned int FRAME_DELAY = 1000 / FPS;

class Engine
{
public:
	Engine(Engine& other) = delete;
	void operator=(const Engine&) = delete;

	static Engine* GetInstance();

	~Engine();

	void InitEngine();
	void ExitEngine();

	void Loop();

	Window* GetWindow() const;
	World* GetWorld() const;
	Input* GetInput() const;
	Time* GetTime() const;
	ResourcePool* GetResourcePool() const;
	Messenger* GetMessenger() const;

	std::string GetProjectDirectory() const;

	unsigned int GetCollisionLayer(std::string layerName);

	sol::state& GetSolState();

private:
	Engine();

	void InitialiseLua();

	static Engine* s_engine;

	sol::state m_lua;

	std::unique_ptr<Window> m_window;
	std::unique_ptr<World> m_world;
	std::unique_ptr<Time> m_time;

	glm::vec4 m_clearColor;

	std::unique_ptr<Input> m_input;
	std::unique_ptr<ResourcePool> m_resourcePool;
	std::unique_ptr<Messenger> m_messenger;

	std::string m_projectDirectory;

	CollisionLayers m_collisionLayers;
#ifdef _DEBUG
	std::array<float, FPS> m_fpsHistory;
	int m_currentFPSIndex = 0;
#endif //_DEBUG
	Uint32 m_frameStart;
	unsigned int m_frameTime;
};

