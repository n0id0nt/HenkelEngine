#include "PhysicsBodyComponent.h"
#include "ECS/Entity/Entity.h"
#include "opengl/openglHelper.h"
#include "../ColliderComponent/ColliderComponent.h"

PhysicsBodyComponent::PhysicsBodyComponent(Entity* entity, PhysicsWorld* world) : Component(entity), m_world(world)
{
	ASSERT(GetEntity()->HasComponent<ColliderComponent>());
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	glm::vec2 position = GetEntity()->GetTransform()->GetWorldPosition();
	bodyDef.position = b2Vec2(position.x, position.y);
	bodyDef.fixedRotation = true;
	bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(entity);

	m_body = m_world->CreateBody(&bodyDef);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = GetEntity()->GetComponent<ColliderComponent>()->GetCollider();
	fixtureDef.density = 1.f;
	fixtureDef.friction = 0.f;
	m_body->CreateFixture(&fixtureDef);
}

PhysicsBodyComponent::~PhysicsBodyComponent()
{
	m_world->DestroyBody(m_body);
}

void PhysicsBodyComponent::UpdatePos()
{
	b2Vec2 vec = m_body->GetPosition();
	GetEntity()->GetTransform()->SetWorldPosition({vec.x, vec.y, 0.f});
}

void PhysicsBodyComponent::SetVelocity(glm::vec2 velocity)
{
	m_body->SetLinearVelocity(b2Vec2(velocity.x, velocity.y));
}

glm::vec2 PhysicsBodyComponent::GetVelocity()
{
	b2Vec2 vec = m_body->GetLinearVelocity();
	return glm::vec2(vec.x, vec.y);
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
