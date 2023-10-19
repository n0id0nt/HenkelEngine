#include "Engine.h"

Engine::Engine(std::string name)
{
	m_window = std::make_unique<Window>(name, 100, 100, 640, 420);
}

void Engine::Loop()
{
	float deltaTime = 0.f;
	while (m_window->IsRunning())
	{
		m_frameStart = SDL_GetTicks();

		m_window->Loop(deltaTime);

		m_frameTime = SDL_GetTicks() - m_frameStart;
		if (FRAME_DELAY > m_frameTime)
		{
			SDL_Delay(FRAME_DELAY - m_frameTime);
		}
		deltaTime = (SDL_GetTicks() - m_frameStart) / 1000.f;
	}
}
