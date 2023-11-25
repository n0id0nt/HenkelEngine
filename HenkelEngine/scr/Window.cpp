#include "Window.h"

#include "Camera.h"
#include "opengl\openglHelper.h"
#include "Input.h"
#include <imgui_internal.h>
#include "opengl\DebugRenderer.h"
#include <imgui_impl_sdl2.h>

Window::Window(std::string name, int x, int y, int w, int h, bool resizableWindow, bool fullscreen)
	: m_running(true), m_width(w), m_height(h)
{
	m_window = SDL_CreateWindow(name.c_str(), x, y, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	//SetResizable(resizableWindow);
	//SetFullscreen(fullscreen);
	m_context = SDL_GL_CreateContext(m_window);
}

Window::~Window()
{
	SDL_GL_DeleteContext(m_context);
	SDL_DestroyWindow(m_window);
}

void Window::SetWindowName(std::string name)
{
	SDL_SetWindowTitle(m_window, name.c_str());
}

void Window::SetFullscreen(bool fullscreen)
{
	SDL_SetWindowFullscreen(m_window, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
}

void Window::SetResizable(bool resizableWindow)
{
	SDL_SetWindowResizable(m_window, resizableWindow ? SDL_TRUE : SDL_FALSE);
}

bool Window::IsRunning()
{
	return m_running;
}

void Window::OnClose()
{
	m_running = false;
}

void Window::Resize(glm::uvec2 size)
{
	m_width = size.x;
	m_height = size.y;
}

int Window::GetWidth()
{
	return m_width;
}

int Window::GetHeight()
{
	return m_height;
}

void Window::SwapBuffers()
{
	SDL_GL_SwapWindow(m_window);
}

void Window::SetUpImGuiForWindow()
{
	ImGui_ImplSDL2_InitForOpenGL(m_window, m_context);
}
