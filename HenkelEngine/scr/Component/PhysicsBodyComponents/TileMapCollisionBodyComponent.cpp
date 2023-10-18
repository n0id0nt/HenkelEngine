#include "TileMapCollisionBodyComponent.h"
#include "../../Entity/Entity.h"
#include "..\..\opengl\DebugRenderer.h"

TileMapCollisionBodyComponent::TileMapCollisionBodyComponent(Entity* entity, b2World* world) : Component(entity), m_world(world)
{
	ASSERT(GetEntity()->HasComponent<TileMapComponent>());

	m_tilemap = GetEntity()->GetComponent<TileMapComponent>();

	b2PolygonShape shape;
	shape.SetAsBox(m_tilemap->GetTileWidth() / 2.f, m_tilemap->GetTileHeight() / 2.f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.friction = 0.f;

	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;

	bodyDef.fixedRotation = true;
	bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(entity);

	for (auto& pos : m_tilemap->GetTilePositions())
	{
		glm::vec2 position = GetEntity()->GetTransform()->GetWorldPosition();
		bodyDef.position = b2Vec2(position.x + pos.x, position.y + pos.y);
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

void TileMapCollisionBodyComponent::Update(float deltaTime)
{
	for (auto& body : m_bodies)
	{
		b2Vec2 vec = body->GetPosition();
		DebugRenderer::DrawRectangle({ vec.x,vec.y,0.f }, m_tilemap->GetTileWidth(), m_tilemap->GetTileHeight());
	}
}
