#pragma once

#include "entt.hpp"

class Engine;

class RenderSystem
{
public:
	RenderSystem(entt::registry* registry, Engine* engine) : m_registry(registry), m_engine(engine)
	{
	}

	void Update();

private:

	entt::registry* m_registry = nullptr;
	Engine* m_engine = nullptr;
};

