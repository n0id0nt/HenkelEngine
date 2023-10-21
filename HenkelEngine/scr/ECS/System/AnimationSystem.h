#pragma once

#include "entt.hpp"

class AnimationSystem
{
public:
	AnimationSystem(entt::registry* registry) : m_registry(registry)
	{
	}

	void Update()
	{

	}

private:

	entt::registry* m_registry;
};

