#include "Messenger.h"
#include "Engine.h"
#include "ECS\Component\ScriptComponent.h"

void Messenger::Send(const std::string& messageId, const sol::object& message, const Entity& sender)
{
	sol::state& lua = Engine::GetInstance()->GetSolState();
	ScriptComponent* senderScript = sender.GetComponent<ScriptComponent>();

	// unbind entity
	senderScript->Unbind(lua);

	// send message to every script component
	std::vector<ScriptComponent*> scriptComponents = Engine::GetInstance()->GetWorld()->GetAllComponents<ScriptComponent>();
	for (auto& scriptComponent : scriptComponents)
	{
		scriptComponent->Bind(lua);
		scriptComponent->OnMessage(messageId, message, sender);
		scriptComponent->Unbind(lua);
	}

	// rebind entity
	senderScript->Bind(lua);
}

void Messenger::LUABind(sol::state& lua)
{
	lua.new_usertype<Messenger>("Messenger",
		"send", &Messenger::Send
	);
	lua.set("Messenger", this);
}