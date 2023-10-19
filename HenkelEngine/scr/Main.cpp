#include "Engine.h"
#include <memory>

int main(int argc, char* argv[])
{
	std::unique_ptr<Engine> app = std::make_unique<Engine>("Henkel Engine");
	app->Loop();

	return 0;
}