#include "ScriptComponent.h"
#include <opengl\openglHelper.h>

ScriptComponent::ScriptComponent(const std::string& script, sol::state& lua, Entity* entity)
	: m_this(), m_entity(entity)
{
	lua.set("Script", this);
	lua.script_file(script);

	if (m_updateFunction)
	{
		lua.script("function update(deltaTime) Script.update(deltaTime) end");
		m_update = lua["update"];
	}
	else
	{
		std::cerr << "Lua script loading no Script.update(deltaTime) function provided" << std::endl;
		ASSERT(false);
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
	lua.set("GO", m_entity);
	lua.set("this", m_this);
	lua.set("Script", this);
}

void ScriptComponent::Unbind(sol::state& lua)
{
	lua.set("GO", sol::nil);
	lua.set("this", sol::nil);
	lua.set("Script", sol::nil);
}

void ScriptComponent::LUABind(sol::state& lua)
{
	lua.new_usertype<ScriptComponent>("Script",
		"update", &ScriptComponent::m_updateFunction
	);
}

