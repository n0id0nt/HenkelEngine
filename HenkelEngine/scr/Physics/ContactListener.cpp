#include "ContactListener.h"
#include <ECS\Component\ScriptComponent.h>
#include "Engine.h"
#include <limits>

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

void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	Entity* entityA = nullptr, *entityB = nullptr;
	GetContactEntities(contact, entityA, entityB);

	TransformComponent* transformA = entityA->GetComponent<TransformComponent>();
	TransformComponent* transformB = entityB->GetComponent<TransformComponent>();
	if (entityA->GetName() == "Player" && entityB->GetName() == "OneWayPlatforms")
	{
		DEBUG_PRINT("Player is A");

		b2Shape* shapeA = contact->GetFixtureA()->GetShape();
		// get lowest point of A
		float highestY = -std::numeric_limits<float>::infinity();
		ASSERT(shapeA->GetType() == b2Shape::e_polygon);
		b2PolygonShape* polygonShapeA = dynamic_cast<b2PolygonShape*>(shapeA);
		if (polygonShapeA)
		{
			auto fixtureABody = contact->GetFixtureA()->GetBody();
			for (int i = 0; i < polygonShapeA->m_count; ++i)
			{
				b2Vec2 vertex = fixtureABody->GetWorldPoint(polygonShapeA->m_vertices[i]);
				if (highestY < vertex.y)
				{
					highestY = vertex.y;
				}
			}
		}

		b2Shape* shapeB = contact->GetFixtureB()->GetShape();
		// get Highest point of B
		float lowestY = std::numeric_limits<float>::infinity();
		ASSERT(shapeB->GetType() == b2Shape::e_chain);
		b2ChainShape* chainShapeB = dynamic_cast<b2ChainShape*>(shapeB);
		if (chainShapeB)
		{
			auto fixtureBBody = contact->GetFixtureB()->GetBody();
			for (int i = 0; i < chainShapeB->m_count; ++i)
			{
				b2Vec2 vertex = fixtureBBody->GetWorldPoint(chainShapeB->m_vertices[i]);
				if (lowestY > vertex.y)
				{
					lowestY = vertex.y;
				}
			}
		}
		
		if (highestY > lowestY)
		{
			contact->SetEnabled(false);
		}
	}
	else if (entityB->GetName() == "Player" && entityA->GetName() == "OneWayPlatforms")
	{
		DEBUG_PRINT("Player is B");
		b2Shape* shapeB = contact->GetFixtureB()->GetShape();
		// get lowest point of A
		float highestY = -std::numeric_limits<float>::infinity();
		ASSERT(shapeB->GetType() == b2Shape::e_polygon);
		b2PolygonShape* polygonShapeB = dynamic_cast<b2PolygonShape*>(shapeB);
		if (polygonShapeB)
		{
			auto fixtureBBody = contact->GetFixtureB()->GetBody();
			for (int i = 0; i < polygonShapeB->m_count; ++i)
			{
				b2Vec2 vertex = fixtureBBody->GetWorldPoint(polygonShapeB->m_vertices[i]);
				if (highestY < vertex.y)
				{
					highestY = vertex.y;
				}
			}
		}

		b2Shape* shapeA = contact->GetFixtureA()->GetShape();
		// get Highest point of B
		float lowestY = std::numeric_limits<float>::max();
		ASSERT(shapeA->GetType() == b2Shape::e_chain);
		b2ChainShape* chainShapeA = dynamic_cast<b2ChainShape*>(shapeA);
		if (chainShapeA)
		{
			auto fixtureABody = contact->GetFixtureA()->GetBody();
			for (int i = 0; i < chainShapeA->m_count; ++i)
			{
				b2Vec2 vertex = fixtureABody->GetWorldPoint(chainShapeA->m_vertices[i]);
				if (lowestY > vertex.y)
				{
					lowestY = vertex.y;
				}
			}
		}

		if (highestY > lowestY)
		{
			contact->SetEnabled(false);
		}
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
	sol::table implulses = lua->create_table();

	b2WorldManifold worldManifold;
	contact->GetWorldManifold(&worldManifold);
	b2Manifold* manifold = contact->GetManifold();

	for (int i = 0; i < manifold->pointCount; ++i) {
		b2Vec2 point = worldManifold.points[i];
		contactPoints.add(glm::vec2(point.x, point.y));
		implulses.add(manifold->points[i].normalImpulse);
	}

	b2Body* bodyA = contact->GetFixtureA()->GetBody();
	b2Body* bodyB = contact->GetFixtureB()->GetBody();
	b2Vec2 relativeVelocity = bodyB->GetLinearVelocity() - bodyA->GetLinearVelocity();

	b2Vec2 normal = worldManifold.normal;

	this->other = other;
	this->contactPoints = contactPoints;
	this->impulses = implulses;
	this->velocity = glm::vec2{relativeVelocity.x, relativeVelocity.y};
	this->normal = glm::vec2{ normal.x, normal.y};
}
