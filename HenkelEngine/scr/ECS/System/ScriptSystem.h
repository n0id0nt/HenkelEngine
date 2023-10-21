#pragma once

#include "entt.hpp"

class ScriptSystem
{
public:
	ScriptSystem(entt::registry* registry) : m_registry(registry)
	{
	}

	void Update()
	{

	}

private:

	entt::registry* m_registry;
};

