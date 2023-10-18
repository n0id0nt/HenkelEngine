#include "StaticBodyComponent.h"
#include "..\..\opengl\openglHelper.h"
#include "glm\glm.hpp"
#include "../../Entity/Entity.h"
#include "../ColliderComponent/ColliderComponent.h"

StaticBodyComponent::StaticBodyComponent(Entity* entity, b2World* world) : Component(entity), m_world(world)
{
	ASSERT(GetEntity()->HasComponent<ColliderComponent>());
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	glm::vec2 position = GetEntity()->GetTransform()->GetWorldPosition();
	bodyDef.position = b2Vec2(position.x, position.y);
	bodyDef.fixedRotation = true;
	bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(entity);

	m_body = m_world->CreateBody(&bodyDef);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = GetEntity()->GetComponent<ColliderComponent>()->GetCollider();
	fixtureDef.friction = 0.f;
	m_body->CreateFixture(&fixtureDef);
}

StaticBodyComponent::~StaticBodyComponent()
{
	m_world->DestroyBody(m_body);
}
