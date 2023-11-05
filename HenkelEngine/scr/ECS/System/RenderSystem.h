#pragma once

#include "ECS/Registry.h"

class RenderSystem
{
public:
	RenderSystem(Registry* registry) : m_registry(registry)
	{
	}

	void Update();

private:

	Registry* m_registry = nullptr;
};

