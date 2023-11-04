#pragma once

#include <string>
#include "sol/sol.hpp"
#include <iostream>
#include "ECS/Registry.h"
#include "ECS\Entity\Entity.h"
#include "ECS\Component\TransformComponent.h"
#include "ECS\Component\PhysicsBodyComponents\PhysicsBodyComponent.h"

class ScriptComponent
{
public:
	ScriptComponent(const std::string& script, sol::state& lua, Entity* entity);

	void Update(float deltaTime);

	void Bind(sol::state& lua);
	void Unbind(sol::state& lua);

private:
	sol::protected_function m_update;

	Entity* m_entity = nullptr;

	sol::table m_this;

};

