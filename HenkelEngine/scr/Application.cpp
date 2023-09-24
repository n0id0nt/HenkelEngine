#include "Application.h"


Application::Application(std::string name)
{
	m_window = std::make_unique<Window>(name, 100, 100, 640, 420);
}

void Application::Loop()
{
	while (m_window->IsRunning())
	{
		m_frameStart = SDL_GetTicks();

		m_window->Loop();

		m_frameTime = SDL_GetTicks() - m_frameStart;
		if (FRAME_DELAY > m_frameTime)
		{
			SDL_Delay(FRAME_DELAY - m_frameTime);
		}
	}
}
