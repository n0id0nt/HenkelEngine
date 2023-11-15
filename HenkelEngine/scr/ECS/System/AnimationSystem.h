#pragma once

#include "ECS/Registry.h"

class AnimationSystem
{
public:
	AnimationSystem(Registry* registry);

	void Update();

private:

	Registry* m_registry = nullptr;
};

