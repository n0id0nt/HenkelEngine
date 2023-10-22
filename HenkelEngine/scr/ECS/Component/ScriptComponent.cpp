#include "ScriptComponent.h"
#include <opengl\openglHelper.h>

ScriptComponent::ScriptComponent(const std::string& script, sol::state& lua)
{
	lua.script_file(script);

	lua.new_usertype<ScriptComponent>("GO",
		"setVelocity", &ScriptComponent::SetVelocity);

	m_update = lua["update"];
	if (m_update.valid())
	{
		std::cerr << "Lua script loading no update(deltaTime) function provided" << std::endl;
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

void ScriptComponent::Bind(sol::state& lua)
{
	lua.set("GO", this);
}

void ScriptComponent::Unbind(sol::state& lua)
{
	lua.set("GO", sol::nil);
}

std::string ScriptComponent::SetVelocity(float x, float y)
{
	return std::string();
}
