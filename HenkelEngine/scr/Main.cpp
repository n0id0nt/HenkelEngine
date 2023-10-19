#include "Engine.h"
#include <memory>

int main(int argc, char* argv[])
{
	std::unique_ptr<Engine> engine = std::make_unique<Engine>();
	engine->Loop();

	return 0;
}