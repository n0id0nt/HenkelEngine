#pragma once

#include "sol\sol.hpp"
#include "LUABindable.h"
#include <string>
#include "ECS\Entity\Entity.h"
#include "ECS/Registry.h"

class ScriptSystem
{
public:
	ScriptSystem(Registry* registry);

	void BindToLua(LUABindable& luaBindable);

	void Update(float deltaTime);

	sol::state& GetSolState();

private:

	Registry* m_registry;
	sol::state m_lua;
};

