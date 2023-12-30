#include "ContactListener.h"
#include <ECS\Component\ScriptComponent.h>
#include <Engine.h>

void ContactListener::BeginContact(b2Contact* contact)
{
	Entity *entityA = nullptr, *entityB = nullptr;
	GetContactEntities(contact, entityA, entityB);
	// call the collision events
	auto scriptComponentA = entityA->GetComponent<ScriptComponent>();
	if (scriptComponentA)
	{
		sol::state& lua = Engine::GetInstance()->GetCurrentScene()->GetLuaState();
		scriptComponentA->Bind(lua); // TODO Might be a little slow to be rebinbinging and unbinding scripts on each collision event think up more efficent way
		scriptComponentA->OnCollisionEnter(entityB);
		scriptComponentA->Unbind(lua);
	}
	auto scriptComponentB = entityB->GetComponent<ScriptComponent>();
	if (scriptComponentB)
	{
		sol::state& lua = Engine::GetInstance()->GetCurrentScene()->GetLuaState();
		scriptComponentB->Bind(lua);
		scriptComponentB->OnCollisionEnter(entityA);
		scriptComponentB->Unbind(lua);
	}
}

void ContactListener::EndContact(b2Contact* contact)
{
	Entity *entityA = nullptr, *entityB = nullptr;
	GetContactEntities(contact, entityA, entityB);
	// call the collision events
	auto scriptComponentA = entityA->GetComponent<ScriptComponent>();
	if (scriptComponentA)
	{
		sol::state& lua = Engine::GetInstance()->GetCurrentScene()->GetLuaState();
		scriptComponentA->Bind(lua); // TODO Might be a little slow to be rebinbinging and unbinding scripts on each collision event think up more efficent way
		scriptComponentA->OnCollisionExit(entityB);
		scriptComponentA->Unbind(lua);
	}
	auto scriptComponentB = entityB->GetComponent<ScriptComponent>();
	if (scriptComponentB)
	{
		sol::state& lua = Engine::GetInstance()->GetCurrentScene()->GetLuaState();
		scriptComponentB->Bind(lua);
		scriptComponentB->OnCollisionExit(entityA);
		scriptComponentB->Unbind(lua);
	}
}

void ContactListener::GetContactEntities(b2Contact* contact, Entity*& entityA, Entity*& entityB)
{
	// Identify fixtures involved in the collision
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	// Use user data to get associated entities
	entityA = reinterpret_cast<Entity*>(fixtureA->GetBody()->GetUserData().pointer);
	entityB = reinterpret_cast<Entity*>(fixtureB->GetBody()->GetUserData().pointer);

}
