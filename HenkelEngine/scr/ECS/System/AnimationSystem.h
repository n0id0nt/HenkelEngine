#pragma once

#include "ECS/Registry.h"

class AnimationSystem
{
public:
	AnimationSystem(Registry* registry) : m_registry(registry)
	{
	}

	void Update()
	{

	}

private:

	Registry* m_registry;
};

