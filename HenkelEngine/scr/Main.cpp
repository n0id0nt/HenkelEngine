#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "Engine.h"
#include <memory>


int main(int argc, char** argv)
{
	Engine::GetInstance()->InitEngine();
	Engine::GetInstance()->Loop();
	Engine::GetInstance()->ExitEngine();

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();

	return 0;
}