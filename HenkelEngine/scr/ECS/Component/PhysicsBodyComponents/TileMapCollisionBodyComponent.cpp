#include "TileMapCollisionBodyComponent.h"

TileMapCollisionBodyComponent::TileMapCollisionBodyComponent(PhysicsWorld* world, b2FixtureDef fixtureDef, b2BodyDef bodyDef, const TileMapComponent& tilemap) 
	: m_world(world), m_bodies()
{
	bodyDef.type = b2_staticBody;

	//m_body = m_world->CreateBody(&bodyDef);
	//m_body->CreateFixture(&fixtureDef);
	b2Vec2 entityPos = bodyDef.position;

	for (auto& pos : tilemap.GetTilePositions())
	{
		bodyDef.position = b2Vec2(entityPos.x + pos.x, entityPos.y + pos.y);
		b2Body* body = m_world->CreateBody(&bodyDef);

		body->CreateFixture(&fixtureDef);
		m_bodies.push_back(body);
	}
}

TileMapCollisionBodyComponent::~TileMapCollisionBodyComponent()
{
	for (auto& body : m_bodies)
		m_world->DestroyBody(body);
}
