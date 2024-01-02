#pragma once

#include <string>
#include "sol/sol.hpp"
#include <iostream>
#include "ECS/Registry.h"
#include "ECS\Entity\Entity.h"
#include "ECS\Component\TransformComponent.h"
#include "ECS\Component\PhysicsBodyComponents\PhysicsBodyComponent.h"
#include <vector>
#include <Physics\ContactListener.h>

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

	void OnCollisionEnter(const ContactListener::Contact& other); // TODO change from other entity to a contact object that is most likely a wrapper for a box2d contact object to give more specific collision info
	void OnCollisionExit(const ContactListener::Contact& other);

	void Bind(sol::state& lua);
	void Unbind(sol::state& lua);

	void AddScriptProperty(const std::string& name, sol::object object);
	void SetScriptProperty(const std::string& name, sol::object object);

	void DrawDebugPanel();

	//void InvokeCollisionEnterEvent();
	//void InvokeCollisionExitEvent();

	//void UpdateEvents();

	static void LUABind(sol::state& lua);

private:
	sol::protected_function m_update;
	std::function<void()> m_updateFunction;

	sol::protected_function m_onCollisionEnter;
	std::function<void(ContactListener::Contact&)> m_onCollisionEnterFunction;
	//bool m_hasCollisionEnterEvent;

	sol::protected_function m_onCollisionExit;
	std::function<void(ContactListener::Contact&)> m_onCollisionExitFunction;
	//bool m_hasCollisionExitEvent;

	Entity* m_entity = nullptr;

	sol::table m_this;

	std::vector<ScriptProperty> m_properties;

	sol::state* m_lua;

};

