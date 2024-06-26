#pragma once

#include "ECS\Registry\Registry.h"
#include "World.h"
#include "ECS/Entity/Entity.h"

namespace LevelSystem
{
	void LoadLevel(World* world, Entity* levelEntity);
	void UnloadLevel(World* world, Entity* levelEntity);
	Entity* FindLevelWithPosition(World* world, Registry* registry, const glm::vec2& position);

#ifdef _DEBUG
	void Update(Registry* registry);
#endif // _DEBUG
};

