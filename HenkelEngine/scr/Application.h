#pragma once

#include <memory>
#include "Window.h"
#include <SDL.h>
#include <string>

const unsigned int FPS = 60;
const unsigned int FRAME_DELAY = 1000 / FPS;

class Application
{
public:
	Application(std::string name);

	void Loop();

private:

	std::unique_ptr<Window> m_window;

	Uint32 m_frameStart;
	unsigned int m_frameTime;
};

