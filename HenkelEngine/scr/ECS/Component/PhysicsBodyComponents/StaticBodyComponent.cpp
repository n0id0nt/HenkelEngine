#include "StaticBodyComponent.h"
#include "opengl\openglHelper.h"
#include "glm\glm.hpp"

StaticBodyComponent::StaticBodyComponent(PhysicsWorld* world, b2FixtureDef fixtureDef, b2BodyDef bodyDef) : m_world(world)
{
	bodyDef.type = b2_staticBody;

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
	return glm::vec2(pos.x, pos.y);
}
