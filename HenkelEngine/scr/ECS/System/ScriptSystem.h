#pragma once

#include "entt.hpp"
#include "sol\sol.hpp"
#include "LUABindable.h"
#include <string>

class ScriptSystem
{
public:
	ScriptSystem(entt::registry* registry);

	void BindToLua(LUABindable& luaBindable);

	void Update(float deltaTime);

	void CreateScriptComponent(const entt::entity& entity, const std::string& file);

private:

	entt::registry* m_registry;
	sol::state m_lua;
};

