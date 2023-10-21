#pragma once

#include "entt.hpp"

class ScriptSystem
{
public:
	ScriptSystem(entt::registry* registry);

	void Update(float deltaTime);

private:

	entt::registry* m_registry;
};

