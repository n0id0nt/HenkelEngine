#pragma once

#include <string>
#include <memory>
#include "Scene.h"

#include <iostream>

#include <GL\glew.h>
#include <SDL_opengl.h>
#include <SDL.h>

class Window
{
public:

	Window(std::string name, int x, int y, int w, int h, bool resizeWindow, bool fullscreen);
	~Window();

	void SetWindowName(std::string name);

	void SetFullscreen(bool fullscreen);
	void SetResizable(bool resizableWindow);

	bool IsRunning();

	void OnClose();
	void Resize(glm::uvec2 size);

	int GetWidth();
	int GetHeight();

	void SwapBuffers();

	void SetUpImGuiForWindow();

private:

	bool m_running;

	int m_width, m_height;

	SDL_Window* m_window;
	SDL_GLContext m_context;
};

