#pragma once

#include <string>
#include "sol/sol.hpp"
#include <iostream>
#include <entt.hpp>

class ScriptComponent
{
public:
	ScriptComponent(const std::string& script, sol::state& lua);

	void Update(float deltaTime);

	void Bind(sol::state& lua);
	void Unbind(sol::state& lua);

	std::string SetVelocity(float x, float y);

private:
	sol::protected_function m_update;

	entt::entity m_enity;
	entt::registry m_registry;
};

