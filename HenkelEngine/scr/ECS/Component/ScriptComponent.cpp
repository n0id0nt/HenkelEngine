#include "ScriptComponent.h"
#include <opengl\openglHelper.h>
#include "imgui.h"

ScriptComponent::ScriptComponent(const std::string& script, sol::state& lua, Entity* entity)
	: m_this(), m_entity(entity), m_properties()
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
	lua.set("Script", sol::nil);
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

	for (auto& propertyPair : m_properties)
	{
		lua.set(propertyPair.first, propertyPair.second);
	}
}

void ScriptComponent::Unbind(sol::state& lua)
{
	lua.set("GO", sol::nil);
	lua.set("this", sol::nil);
	lua.set("Script", sol::nil);

	for (auto& propertyPair : m_properties)
	{
		propertyPair.second = lua[propertyPair.first];
		lua.set(propertyPair.first, sol::nil);
	}
}

void ScriptComponent::AddScriptProperty(const std::string& name, sol::object object)
{
	m_properties[name] = object;
}

void ScriptComponent::DrawDebugPanel()
{
	const float inputFieldWidth = 80.f;
	ImGui::Text("Script Component:");

	for (auto& propertyPair : m_properties)
	{
		switch (propertyPair.second.get_type())
		{
		case sol::type::number:
		{
			float value = propertyPair.second.as<float>();
			ImGui::InputFloat(propertyPair.first.c_str(), &value);
			break;
		}
		case sol::type::string:
		{
			std::string stringValue = propertyPair.second.as<std::string>();
			// Copy the string to a char array
			char inputBuffer[256];
			strncpy_s(inputBuffer, stringValue.c_str(), sizeof(inputBuffer));
			ImGui::InputText(propertyPair.first.c_str(), inputBuffer, sizeof(inputBuffer));
			break;
		}
		case sol::type::boolean:
		{
			bool value = propertyPair.second.as<bool>();
			ImGui::Checkbox(propertyPair.first.c_str(), &value);
			break;
		}
		default:
			ImGui::Text((propertyPair.first + std::string(" is not a supported data type")).c_str());
			break;
		}
	}
}

void ScriptComponent::LUABind(sol::state& lua)
{
	lua.new_usertype<ScriptComponent>("Script",
		"property", &ScriptComponent::AddScriptProperty,
		"update", &ScriptComponent::m_updateFunction
	);
}

