#define _CRTDBG_MAP_ALLOC
#define SDL_MAIN_HANDLED
#include "Engine.h"

int main(int argc, char** argv)
{
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);

	Engine::GetInstance()->InitEngine();
	Engine::GetInstance()->Loop();
	Engine::GetInstance()->ExitEngine();

	return 0;
}