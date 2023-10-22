#pragma once

#include <memory>
#include "Window.h"
#include <SDL.h>
#include <string>

#include "glm\glm.hpp"
#include "Input.h"
#include "ResourcePool.h"

const unsigned int FPS = 60;
const unsigned int FRAME_DELAY = 1000 / FPS;

class Engine
{
public:
	Engine();
	~Engine();

	void InitEngine();

	void Loop();

	Window* GetWindow() const;
	Scene* GetCurrentScene() const;
	Input* GetInput() const;
	ResourcePool* GetResourcePool() const;

private:

	std::unique_ptr<Window> m_window;
	std::unique_ptr<Scene> m_scene;

	glm::vec4 m_clearColor;

	std::unique_ptr<Input> m_input;
	std::unique_ptr<ResourcePool> m_resourcePool;

	Uint32 m_frameStart;
	unsigned int m_frameTime;
};

