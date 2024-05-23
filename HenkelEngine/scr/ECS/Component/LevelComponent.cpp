#include "LevelComponent.h"

LevelComponent::LevelComponent(const std::string& fileName, glm::ivec2 offset, glm::ivec2 size)
	: fileName(fileName), offset(offset), size(size)
{
}
