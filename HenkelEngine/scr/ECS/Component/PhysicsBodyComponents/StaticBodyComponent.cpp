#include "StaticBodyComponent.h"
#include "opengl\openglHelper.h"
#include "glm\glm.hpp"

StaticBodyComponent::StaticBodyComponent(PhysicsWorld* world, glm::vec2 collisionShape, Entity* entity, bool isSensor) : m_world(world), m_collisionShape(collisionShape)
{
	b2BodyDef bodyDef;
	bodyDef.fixedRotation = true;
	bodyDef.type = b2_staticBody;
	bodyDef.position = b2Vec2(bodyDef.position.x / m_world->GetPixelsPerMeter(), bodyDef.position.y / m_world->GetPixelsPerMeter());
	bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(entity);

	b2FixtureDef fixtureDef;
	b2PolygonShape shape;
	shape.SetAsBox(collisionShape.x / (2.f * m_world->GetPixelsPerMeter()), collisionShape.y / (2.f * m_world->GetPixelsPerMeter()));
	fixtureDef.shape = &shape;
	fixtureDef.friction = 0.f;
	fixtureDef.density = 1.f;
	fixtureDef.isSensor = isSensor;

	m_body = m_world->CreateBody(&bodyDef);
	m_body->CreateFixture(&fixtureDef);
}

StaticBodyComponent::~StaticBodyComponent()
{
	m_world->DestroyBody(m_body);
}

glm::vec2 StaticBodyComponent::GetPosition()
{
	b2Vec2 pos = m_body->GetPosition();
	return glm::vec2(pos.x * m_world->GetPixelsPerMeter(), pos.y * m_world->GetPixelsPerMeter());
}

void StaticBodyComponent::SetPosition(glm::vec2 pos)
{
	m_body->SetTransform(b2Vec2(pos.x / m_world->GetPixelsPerMeter(), pos.y / m_world->GetPixelsPerMeter()), 0.f);
}

glm::vec2 StaticBodyComponent::GetCollisionShape()
{
	return m_collisionShape;
}
