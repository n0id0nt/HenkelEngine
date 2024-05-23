#pragma once

#include "glm\glm.hpp"
#include <string>

struct LevelComponent
{
	LevelComponent(const std::string& fileName, glm::ivec2 offset, glm::ivec2 size);

	std::string fileName;

	glm::ivec2 offset;
	glm::ivec2 size;
};

