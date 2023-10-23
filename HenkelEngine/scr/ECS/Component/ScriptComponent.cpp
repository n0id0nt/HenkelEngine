#include "ScriptComponent.h"
#include <opengl\openglHelper.h>

#include "ECS\Component\TransformComponent.h"
#include "ECS\Component\PhysicsBodyComponents\PhysicsBodyComponent.h"

ScriptComponent::ScriptComponent(const std::string& script, sol::state& lua, entt::entity entt, entt::registry* registry) 
	: m_this(), m_enity(entt), m_registry(registry)
{
	lua.script_file(script);

	m_update = lua["update"];
	if (m_update.valid())
	{
		std::cerr << "Lua script loading no update(deltaTime) function provided" << std::endl;
	}

	// bind the bindable components
	auto* transform = m_registry->try_get<TransformComponent>(m_enity);
	if (transform)
	{
		m_this["transform"] = transform;
	}
	auto* physicsBody = m_registry->try_get<PhysicsBodyComponent>(m_enity);
	if (physicsBody)
	{
		m_this["physicsBody"] = physicsBody;
	}

}

void ScriptComponent::Update(float deltaTime)
{
	if (m_update.valid())
	{
		sol::protected_function updateScript = m_update;

		sol::protected_function_result result = updateScript(deltaTime);

		// Check if the execution was successful
		if (!result.valid()) {
			// An error occurred; retrieve and handle the error message
			sol::error err = result;
			std::cerr << "Lua script execution error: " << err.what() << std::endl;

			ASSERT(false);
		}
	}
}

void ScriptComponent::LUABind(sol::state& lua)
{
	lua.new_usertype<ScriptComponent>("GO",
		"setVelocity", &ScriptComponent::Test);
}

void ScriptComponent::Bind(sol::state& lua)
{
	lua.set("GO", this);
	lua.set("this", m_this);
}

void ScriptComponent::Unbind(sol::state& lua)
{
	lua.set("GO", sol::nil);
	lua.set("this", sol::nil);
}