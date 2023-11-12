#pragma once

#include <string>
#include "sol/sol.hpp"
#include <iostream>
#include "ECS/Registry.h"
#include "ECS\Entity\Entity.h"
#include "ECS\Component\TransformComponent.h"
#include "ECS\Component\PhysicsBodyComponents\PhysicsBodyComponent.h"
#include <vector>

struct ScriptProperty
{
	std::string name;
	sol::object value;
};

class ScriptComponent
{
public:
	ScriptComponent(const std::string& script, sol::state& lua, Entity* entity);

	void Update();

	void Bind(sol::state& lua);
	void Unbind(sol::state& lua);

	void AddScriptProperty(const std::string& name, sol::object object);
	void SetScriptProperty(const std::string& name, sol::object object);

	void DrawDebugPanel();

	static void LUABind(sol::state& lua);

private:
	sol::protected_function m_update;
	std::function<void()> m_updateFunction;

	Entity* m_entity = nullptr;

	sol::table m_this;

	std::vector<ScriptProperty> m_properties;

	sol::state* m_lua;

};

