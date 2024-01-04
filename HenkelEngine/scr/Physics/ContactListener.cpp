#include "ContactListener.h"
#include <ECS\Component\ScriptComponent.h>
#include <Engine.h>

void ContactListener::BeginContact(b2Contact* contact)
{
	if (!contact->IsTouching()) return;

	Entity *entityA = nullptr, *entityB = nullptr;
	GetContactEntities(contact, entityA, entityB);
	// call the collision events
	auto scriptComponentA = entityA->GetComponent<ScriptComponent>();
	if (scriptComponentA)
	{
		sol::state& lua = Engine::GetInstance()->GetCurrentScene()->GetLuaState();
		Contact contactStruct(entityA, contact, &lua);
		scriptComponentA->Bind(lua); // TODO Might be a little slow to be rebinbinging and unbinding scripts on each collision event think up more efficent way
		scriptComponentA->OnCollisionEnter(contactStruct);
		scriptComponentA->Unbind(lua);
	}
	auto scriptComponentB = entityB->GetComponent<ScriptComponent>();
	if (scriptComponentB)
	{
		sol::state& lua = Engine::GetInstance()->GetCurrentScene()->GetLuaState();
		Contact contactStruct(entityA, contact, &lua);
		scriptComponentB->Bind(lua);
		scriptComponentB->OnCollisionEnter(contactStruct);
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
		Contact contactStruct(entityA, contact, &lua);
		scriptComponentA->Bind(lua); // TODO Might be a little slow to be rebinbinging and unbinding scripts on each collision event think up more efficent way
		scriptComponentA->OnCollisionExit(contactStruct);
		scriptComponentA->Unbind(lua);
	}
	auto scriptComponentB = entityB->GetComponent<ScriptComponent>();
	if (scriptComponentB)
	{
		sol::state& lua = Engine::GetInstance()->GetCurrentScene()->GetLuaState();
		Contact contactStruct(entityA, contact, &lua);
		scriptComponentB->Bind(lua);
		scriptComponentB->OnCollisionExit(contactStruct);
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

ContactListener::Contact::Contact(Entity* other, b2Contact* contact, sol::state* lua)
{
	sol::table contactPoints = lua->create_table();

	b2WorldManifold worldManifold;
	contact->GetWorldManifold(&worldManifold);

	for (int i = 0; i < contact->GetManifold()->pointCount; ++i) {
		b2Vec2 point = worldManifold.points[i];
		contactPoints.add(glm::vec2(point.x, point.y));
	}

	this->other = other;
	this->contactPoints = contactPoints;
	impulse = 0.f;
	velocity = 0.f;
}
