#include "Engine.h"
#include <memory>

int main(int argc, char** argv)
{
	Engine::GetInstance()->InitEngine();
	Engine::GetInstance()->Loop();
	Engine::GetInstance()->ExitEngine();

	return 0;
}