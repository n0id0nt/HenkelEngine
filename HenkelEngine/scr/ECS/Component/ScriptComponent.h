#pragma once

#include <string>
#include "sol/sol.hpp"
#include <iostream>
#include <entt.hpp>

#include "ECS\Component\TransformComponent.h"
#include "ECS\Component\PhysicsBodyComponents\PhysicsBodyComponent.h"

class ScriptComponent
{
public:
	ScriptComponent(const std::string& script, sol::state& lua, entt::entity entt, entt::registry* registry);

	void Update(float deltaTime);

	static void LUABind(sol::state& lua);

	void Bind(sol::state& lua);
	void Unbind(sol::state& lua);

	std::string Test() { return "test"; }

	TransformComponent* GetTransform();
	PhysicsBodyComponent* GetPhysicsBody();

private:
	sol::protected_function m_update;

	entt::entity m_enity;
	entt::registry* m_registry;

	sol::table m_this;

};

