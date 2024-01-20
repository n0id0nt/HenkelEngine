#include "PhysicsBodyComponent.h"
#include "opengl/openglHelper.h"
#include <glm\gtx\vector_angle.hpp>
#include <opengl\DebugRenderer.h>
#include <ECS\Entity\Entity.h>

PhysicsBodyComponent::PhysicsBodyComponent(PhysicsWorld* world, glm::vec2 collisionShape, Entity* entity, bool isSensor, uint16 categoryBits, uint16 maskBits) : m_world(world), m_collisionShape(collisionShape)
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

void PhysicsBodyComponent::SetPosition(glm::vec2 pos)
{
	m_body->SetTransform(b2Vec2(pos.x / m_world->GetPixelsPerMeter(), pos.y / m_world->GetPixelsPerMeter()), 0.f);
}

glm::vec2 PhysicsBodyComponent::GetCollisionShape()
{
	return m_collisionShape;
}

//TODO move this to a helper class
bool IsBodySensor(b2Body* body) {
	// Iterate through all fixtures of the body
	for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
		// Check if the fixture is a sensor
		if (fixture->IsSensor()) {
			return true; // At least one sensor fixture found
		}
	}
	return false; // No sensor fixtures found
}

bool PhysicsBodyComponent::CheckCollisionAtAngle(float angle, float groundAngleBuffer)
{
	for (auto& contact : GetContacts())
	{
		if (contact->IsTouching())
		{
			b2Vec2 normal = contact->GetManifold()->localNormal;
			
			bool isFixtureA = contact->GetFixtureA()->GetBody() == m_body;
			b2Body* other = isFixtureA ? contact->GetFixtureB()->GetBody() : contact->GetFixtureA()->GetBody();
			if (IsBodySensor(other)) continue;
			float angleRadians = glm::radians(angle);
			float angle = glm::degrees(glm::angle(glm::vec2{ glm::cos(angleRadians), glm::sin(angleRadians) }, (isFixtureA ? 1.f : -1.f) * glm::vec2{ normal.x, normal.y }));
			if (angle < groundAngleBuffer)
			{
				glm::vec3 pos = glm::vec3{ m_body->GetPosition().x, m_body->GetPosition().y, 0 } * m_world->GetPixelsPerMeter();
				DebugRenderer::DrawLine(pos, glm::vec3{ normal.x * 16.f, normal.y * 16.f, 0.f } + pos, { 0.9f,0.1f,0.2f });
				return true;
			}
		}
	}
	return false;
}

bool PhysicsBodyComponent::CheckGrounded(float groundAngleBuffer)
{
	const float angle = 90.f;
	return CheckCollisionAtAngle(angle, groundAngleBuffer);
}

unsigned int PhysicsBodyComponent::GetCollisionLayersMask()
{
	return m_body->GetFixtureList()->GetFilterData().maskBits;
}

std::set<std::string> PhysicsBodyComponent::GetCollisionLayers()
{
	return std::set<std::string>();
}

sol::table PhysicsBodyComponent::LUAGetCollisionLayers()
{
	return sol::table();
}

void PhysicsBodyComponent::LUABind(sol::state& lua)
{
	lua.new_usertype<PhysicsBodyComponent>("physicsBody",
		"setVelocity", &PhysicsBodyComponent::SetVelocity,
		"getVelocity", &PhysicsBodyComponent::GetVelocity,
		"checkGrounded", &PhysicsBodyComponent::CheckGrounded,
		"checkCollisionAtAngle", &PhysicsBodyComponent::CheckCollisionAtAngle
	);
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
