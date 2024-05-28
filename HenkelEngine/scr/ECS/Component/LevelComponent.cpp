#include "LevelComponent.h"

LevelComponent::LevelComponent(const std::string& fileName, glm::ivec2 size)
	: fileName(fileName), size(size), loaded(false)
{
}

void LevelComponent::LUABind(sol::state& lua)
{
	lua.new_usertype<LevelComponent>("LevelComponent"
		,"fileName", &LevelComponent::fileName
		,"size", &LevelComponent::size
		,"loaded", &LevelComponent::loaded
	);
}
