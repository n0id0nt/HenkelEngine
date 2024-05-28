#pragma once

#include "glm\glm.hpp"
#include <string>
#include "sol\sol.hpp"

struct LevelComponent
{
	LevelComponent(const std::string& fileName, glm::ivec2 size);

	std::string fileName;
	glm::ivec2 size;
	bool loaded;

	static void LUABind(sol::state& lua);
};

