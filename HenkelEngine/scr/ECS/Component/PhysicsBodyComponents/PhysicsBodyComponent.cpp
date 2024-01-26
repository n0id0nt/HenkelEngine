#include "PhysicsBodyComponent.h"
#include "opengl/openglHelper.h"
#include <glm\gtx\vector_angle.hpp>
#include <opengl\DebugRenderer.h>
#include <ECS\Entity\Entity.h>

PhysicsBodyComponent::PhysicsBodyComponent(PhysicsWorld* world, glm::vec2 collisionShape, Entity* entity, bool isSensor, uint16 categoryBits, uint16 maskBits) : CollisionBodyComponent(world)
{
	b2Filter filter;
	filter.categoryBits = categoryBits;
	filter.maskBits = maskBits;

	b2BodyDef bodyDef;
	bodyDef.fixedRotation = true;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = b2Vec2(bodyDef.position.x / m_world->GetPixelsPerMeter(), bodyDef.position.y / m_world->GetPixelsPerMeter());
	bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(entity);

	b2FixtureDef fixtureDef;
	b2PolygonShape shape;
	shape.SetAsBox(collisionShape.x / (2.f * m_world->GetPixelsPerMeter()), collisionShape.y / (2.f * m_world->GetPixelsPerMeter()));
	fixtureDef.shape = &shape;
	fixtureDef.friction = 0.f;
	fixtureDef.density = 1.f;
	fixtureDef.isSensor = isSensor;
	fixtureDef.filter = filter;

	m_body = m_world->CreateBody(&bodyDef);
	m_body->CreateFixture(&fixtureDef);
}

void PhysicsBodyComponent::LUABind(sol::state& lua)
{
	lua.new_usertype<PhysicsBodyComponent>("physicsBody",
		sol::base_classes, sol::bases<CollisionBodyComponent>()
	);
}
