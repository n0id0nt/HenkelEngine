#pragma once

#include <string>
#include "sol/sol.hpp"
#include <iostream>
#include <entt.hpp>
#include "ECS\Entity\Entity.h"
#include "ECS\Component\TransformComponent.h"
#include "ECS\Component\PhysicsBodyComponents\PhysicsBodyComponent.h"

class ScriptComponent
{
public:
	ScriptComponent(const std::string& script, sol::state& lua, Entity* entity, entt::registry* registry);

	void Update(float deltaTime);

	void Bind(sol::state& lua);
	void Unbind(sol::state& lua);

	std::string Test() { return "test"; }

private:
	sol::protected_function m_update;

	Entity* m_entity = nullptr;
	entt::registry* m_registry = nullptr;

	sol::table m_this;

};

