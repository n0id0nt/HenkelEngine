#include "ScriptComponent.h"
#include <opengl\openglHelper.h>
#include "imgui.h"
#include <DebugGUIPanels\ImGuiHelper.h>

ScriptComponent::ScriptComponent(const std::string& script, sol::state& lua, Entity* entity)
	: m_this(), m_entity(entity), m_properties(), m_lua(&lua)//, m_hasCollisionEnterEvent(false), m_hasCollisionExitEvent(false)
{
	lua.set("Script", this);
	lua.script_file(script);

	if (m_updateFunction)
	{
		lua.script("function update() Script.update() end");
		m_update = lua["update"];
	}
	else
	{
		DEBUG_ERROR("Lua script loading no Script.update() function provided");
		ASSERT(false); // TODO can probaly remove this check cause the update function is not always needed
	}

	if (m_onCollisionEnterFunction)
	{
		lua.script("function onCollisionEnter(contact) Script.onCollisionEnter(contact) end");
		m_onCollisionEnter = lua["onCollisionEnter"];
	}

	if (m_onCollisionExitFunction)
	{
		lua.script("function onCollisionExit(contact) Script.onCollisionExit(contact) end");

	}

	if (m_onCollisionPreSolveFunction)
	{
		lua.script("function onCollisionPreSolve(contact) Script.onCollisionPreSolve(contact) end");
		m_onCollisionPreSolve = lua["onCollisionPreSolve"];
	}

	if (m_onMessageFunction)
	{
		lua.script("function onMessage(messageId, message, sender) Script.onMessage(messageId, message, sender) end");
		m_onMessage = lua["onMessage"];
	}
	lua.set("Script", sol::nil);
}

void ScriptComponent::Update()
{
	if (m_update.valid())
	{
		sol::protected_function updateScript = m_update;

		sol::protected_function_result result = updateScript();

		// Check if the execution was successful
		if (!result.valid()) {
			// An error occurred; retrieve and handle the error message
			sol::error err = result;
			DEBUG_ERROR(std::format("Lua script execution error: {}", err.what()));

			ASSERT(false);
		}
	}
}

void ScriptComponent::OnCollisionEnter(ContactListener::Contact& contact)
{
	if (m_onCollisionEnter.valid())
	{
		sol::protected_function onCollisionEnterScript = m_onCollisionEnter;

		sol::protected_function_result result = onCollisionEnterScript(contact);

		// Check if the execution was successful
		if (!result.valid()) {
			// An error occurred; retrieve and handle the error message
			sol::error err = result;
			DEBUG_ERROR(std::format("Lua script execution error: {}", err.what()));

			ASSERT(false);
		}
	}
}

void ScriptComponent::OnCollisionExit(ContactListener::Contact& contact)
{
	if (m_onCollisionExit.valid())
	{
		sol::protected_function onCollisionExitScript = m_onCollisionExit;

		sol::protected_function_result result = onCollisionExitScript(contact);

		// Check if the execution was successful
		if (!result.valid()) {
			// An error occurred; retrieve and handle the error message
			sol::error err = result;
			DEBUG_ERROR(std::format("Lua script execution error: {}", err.what()));

			ASSERT(false);
		}
	}
}

void ScriptComponent::OnCollisionPreSolve(ContactListener::Contact& contact)
{
	if (m_onCollisionPreSolve.valid())
	{
		sol::protected_function onCollisionPreSolveScript = m_onCollisionPreSolve;

		sol::protected_function_result result = onCollisionPreSolveScript(contact);

		// Check if the execution was successful
		if (!result.valid()) {
			// An error occurred; retrieve and handle the error message
			sol::error err = result;
			DEBUG_ERROR(std::format("Lua script execution error: {}", err.what()));

			ASSERT(false);
		}
	}
}

void ScriptComponent::OnMessage(const std::string& messageId, const sol::object& message, const Entity& sender)
{
	if (m_onMessage.valid())
	{
		sol::protected_function onMessageScript = m_onMessage;

		sol::protected_function_result result = onMessageScript(messageId, message, &sender);

		// Check if the execution was successful
		if (!result.valid()) {
			// An error occurred; retrieve and handle the error message
			sol::error err = result;
			DEBUG_ERROR(std::format("Lua script execution error: {}", err.what()));

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
		lua.set(propertyPair.name, propertyPair.value);
	}
}

void ScriptComponent::Unbind(sol::state& lua)
{
	lua.set("GO", sol::nil);
	lua.set("this", sol::nil);
	lua.set("Script", sol::nil);

	for (auto& propertyPair : m_properties)
	{
		propertyPair.value = lua[propertyPair.name];
		lua.set(propertyPair.name, sol::nil);
	}
}

void ScriptComponent::AddScriptProperty(const std::string& name, sol::object object)
{
	m_properties.push_back({name, object});
}

void ScriptComponent::SetScriptProperty(const std::string& name, sol::object object)
{
	auto it = std::find_if(m_properties.begin(), m_properties.end(), [name](const ScriptProperty& scriptProperty) { return scriptProperty.name == name; });

	// Check if the component was found
	ASSERT(it != m_properties.end());
	if (it == m_properties.end()) return;
	it->name = name;
	it->value = object;
}

void ScriptComponent::DrawDebugPanel()
{
	const float inputFieldWidth = 80.f;
	ImGui::Text("Script Component:");

	for (auto& propertyPair : m_properties)
	{
		switch (propertyPair.value.get_type())
		{
		case sol::type::number:
		{
			float value = propertyPair.value.as<float>();
			ImGui::DragFloat(propertyPair.name.c_str(), &value);
			(*m_lua)["temp"] = value;
			propertyPair.value = (*m_lua)["temp"];
			break;
		}
		case sol::type::string:
		{
			std::string stringValue = propertyPair.value.as<std::string>();
			// Copy the string to a char array
			char inputBuffer[256];
			strncpy_s(inputBuffer, stringValue.c_str(), sizeof(inputBuffer));
			ImGui::InputText(propertyPair.name.c_str(), inputBuffer, sizeof(inputBuffer));
			(*m_lua)["temp"] = stringValue;
			propertyPair.value = (*m_lua)["temp"];
			break;
		}
		case sol::type::boolean:
		{
			bool value = propertyPair.value.as<bool>();
			ImGui::Checkbox(propertyPair.name.c_str(), &value);
			(*m_lua)["temp"] = value;
			propertyPair.value = (*m_lua)["temp"];
			break;
		}
		case sol::type::userdata:
		{
			if (propertyPair.value.is<glm::vec2>())
			{
				glm::vec2 value = propertyPair.value.as<glm::vec2>();
				ImGui::DrawVec2Control(propertyPair.name.c_str(), value);
				(*m_lua)["temp"] = value;
				propertyPair.value = (*m_lua)["temp"];
			}
			else
			{
				ImGui::Text((propertyPair.name + std::string(" is not a supported userdata type")).c_str());
			}
			break;
		}
		default:
			ImGui::Text((propertyPair.name + std::string(" is not a supported data type")).c_str());
			break;
		}
	}
}

//void ScriptComponent::InvokeCollisionEnterEvent()
//{
//	m_hasCollisionEnterEvent = true;
//}
//
//void ScriptComponent::InvokeCollisionExitEvent()
//{
//	m_hasCollisionExitEvent = true;
//}
//
//void ScriptComponent::UpdateEvents()
//{
//	if (m_hasCollisionEnterEvent)
//	{
//		OnCollisionEnter();
//	}
//	if (m_hasCollisionExitEvent)
//	{
//		OnCollisionExit();
//	}
//}

void ScriptComponent::LUABind(sol::state& lua)
{
	lua.new_usertype<ScriptComponent>("Script",
		"property", &ScriptComponent::AddScriptProperty,
		"update", &ScriptComponent::m_updateFunction,
		"onCollisionEnter", &ScriptComponent::m_onCollisionEnterFunction,
		"onCollisionExit", &ScriptComponent::m_onCollisionExitFunction,
		"onCollisionPreSolve", &ScriptComponent::m_onCollisionPreSolveFunction,
		"onMessage", &ScriptComponent::m_onMessage
	);
}

