#include "StaticBodyComponent.h"
#include "opengl\openglHelper.h"
#include "glm\glm.hpp"

StaticBodyComponent::StaticBodyComponent(PhysicsWorld* world, glm::vec2 collisionShape, Entity* entity, bool isSensor, uint16 categoryBits, uint16 maskBits) : CollisionBodyComponent(world)
{
	b2Filter filter;
	filter.categoryBits = categoryBits;
	filter.maskBits = maskBits;

	b2BodyDef bodyDef;
	bodyDef.fixedRotation = true;
	bodyDef.type = b2_staticBody;
	bodyDef.position = b2Vec2(bodyDef.position.x / m_world->GetPixelsPerMeter(), bodyDef.position.y / m_world->GetPixelsPerMeter());
	bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(entity);

	b2FixtureDef fixtureDef;
	b2PolygonShape shape;
	m_collisionShape = collisionShape;
	shape.SetAsBox(collisionShape.x / (2.f * m_world->GetPixelsPerMeter()), collisionShape.y / (2.f * m_world->GetPixelsPerMeter()));
	fixtureDef.shape = &shape;
	fixtureDef.friction = 0.f;
	fixtureDef.density = 1.f;
	fixtureDef.isSensor = isSensor;
	fixtureDef.filter = filter;

	m_body = m_world->CreateBody(&bodyDef);
	m_body->CreateFixture(&fixtureDef);
}

void StaticBodyComponent::LUABind(sol::state& lua)
{
	lua.new_usertype<StaticBodyComponent>("staticBodyComponent",
		sol::base_classes, sol::bases<CollisionBodyComponent>()
	);
}
