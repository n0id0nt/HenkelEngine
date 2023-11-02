#pragma once

#include "ECS/Registry.h"

class Engine;

class RenderSystem
{
public:
	RenderSystem(Registry* registry, Engine* engine) : m_registry(registry), m_engine(engine)
	{
	}

	void Update();

private:

	Registry* m_registry = nullptr;
	Engine* m_engine = nullptr;
};

