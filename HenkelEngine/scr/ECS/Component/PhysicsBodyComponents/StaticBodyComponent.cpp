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
