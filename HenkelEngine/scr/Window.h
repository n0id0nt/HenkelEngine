#pragma once

#include <string>
#include <memory>
#include "Scene.h"

#include <iostream>

#include <GL\glew.h>
#include <vector>
#include <SDL_opengl.h>

#include "glm\glm.hpp"
#include "Input.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include "Event.h"


class Window
{
public:

	Window(std::string name, int x, int y, int w, int h);
	~Window();

	void Input();

	void Loop(float deltaTime);

	void OnClose();
	void OnResize(int w, int h);

	bool IsRunning() const { return m_running; }

	int Width, Height;

	SDL_Window* GetSDLWindow() const { return m_window; }
	SDL_GLContext* GetSDLContext() { return &m_context; }

private:
	SDL_Window* m_window;
	SDL_GLContext m_context;

	bool m_running;
	std::unique_ptr<Scene> m_scene;

	glm::vec4 m_clearColor;
};

