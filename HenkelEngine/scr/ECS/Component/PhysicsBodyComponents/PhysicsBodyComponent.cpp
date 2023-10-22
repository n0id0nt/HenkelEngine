#include "PhysicsBodyComponent.h"
#include "opengl/openglHelper.h"
#include <glm\gtx\vector_angle.hpp>

PhysicsBodyComponent::PhysicsBodyComponent(PhysicsWorld* world, b2FixtureDef fixtureDef, b2BodyDef bodyDef) : m_world(world)
{
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = b2Vec2(bodyDef.position.x / m_world->GetPixelsPerMeter(), bodyDef.position.y / m_world->GetPixelsPerMeter());

	m_body = m_world->CreateBody(&bodyDef);
	m_body->CreateFixture(&fixtureDef);
}

PhysicsBodyComponent::~PhysicsBodyComponent()
{
	m_world->DestroyBody(m_body);
}

void PhysicsBodyComponent::SetVelocity(glm::vec2 velocity)
{
	m_body->SetLinearVelocity(b2Vec2(velocity.x / m_world->GetPixelsPerMeter(), velocity.y / m_world->GetPixelsPerMeter()));
}

glm::vec2 PhysicsBodyComponent::GetVelocity()
{
	b2Vec2 vec = m_body->GetLinearVelocity();
	return glm::vec2(vec.x * m_world->GetPixelsPerMeter(), vec.y * m_world->GetPixelsPerMeter());
}

glm::vec2 PhysicsBodyComponent::GetPosition()
{
	b2Vec2 pos = m_body->GetPosition();
	return glm::vec2(pos.x * m_world->GetPixelsPerMeter(), pos.y * m_world->GetPixelsPerMeter());
}

bool PhysicsBodyComponent::CheckGrounded(float groundAngle)
{
	for (auto& contact : GetContacts())
	{
		if (contact->IsTouching())
		{
			b2Vec2 normal = contact->GetManifold()->localNormal;
			//glm::vec3 pos = GetEntity()->GetTransform()->GetWorldPosition();
			//DebugRenderer::DrawLine(pos, glm::vec3{ normal.x * 8.f, normal.y * 8.f, 0.f } + pos, {1.f,0.f,0.f});
			bool isFixtureA = contact->GetFixtureA()->GetBody() == m_body;
			float angle = glm::degrees(glm::angle(glm::vec2{ 0.f, 1.f }, (isFixtureA ? 1.f : -1.f)* glm::vec2{ normal.x, normal.y }));
			if (angle < groundAngle)
			{
				return true;
			}
		}
	}
	return false;
}

std::vector<b2Contact*> PhysicsBodyComponent::GetContacts()
{
	std::vector<b2Contact*> contacts;
	for (b2ContactEdge* ce = m_body->GetContactList(); ce; ce = ce->next)
	{
		contacts.push_back(ce->contact);
	}
	return contacts;
}
