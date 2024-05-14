#include "CollisionBodyComponent.h"
#include <glm\gtx\vector_angle.hpp>
#include <opengl\DebugRenderer.h>

CollisionBodyComponent::CollisionBodyComponent(PhysicsWorld* world) : m_world(world)
{
}

CollisionBodyComponent::~CollisionBodyComponent()
{
	m_world->DestroyBody(m_body);
}

b2Body* CollisionBodyComponent::GetBody() const
{
	return m_body;
}

void CollisionBodyComponent::SetVelocity(glm::vec2 velocity)
{
	m_body->SetLinearVelocity(b2Vec2(velocity.x / m_world->GetPixelsPerMeter(), velocity.y / m_world->GetPixelsPerMeter()));
}

glm::vec2 CollisionBodyComponent::GetVelocity()
{
	b2Vec2 vec = m_body->GetLinearVelocity();
	return glm::vec2(vec.x * m_world->GetPixelsPerMeter(), vec.y * m_world->GetPixelsPerMeter());
}

glm::vec2 CollisionBodyComponent::GetPosition()
{
	b2Vec2 pos = m_body->GetPosition();
	return glm::vec2(pos.x * m_world->GetPixelsPerMeter(), pos.y * m_world->GetPixelsPerMeter());
}

void CollisionBodyComponent::SetPosition(glm::vec2 pos)
{
	m_body->SetTransform(b2Vec2(pos.x / m_world->GetPixelsPerMeter(), pos.y / m_world->GetPixelsPerMeter()), 0.f);
}

glm::vec2 CollisionBodyComponent::GetCollisionShape()
{
	return m_collisionShape;
}

bool CollisionBodyComponent::CheckCollisionAtAngle(float angle, float groundAngleBuffer)
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
			float angle = glm::degrees(glm::angle(glm::vec2{ glm::cos(angleRadians), glm::sin(angleRadians) }, (isFixtureA ? 1.f : -1.f)* glm::vec2{ normal.x, normal.y }));
			if (angle < groundAngleBuffer)
			{
#ifdef _DEBUG
				glm::vec3 pos = glm::vec3{ m_body->GetPosition().x, m_body->GetPosition().y, 0 } * m_world->GetPixelsPerMeter();
				DebugRenderer::DrawLine(pos, glm::vec3{ normal.x * 16.f, normal.y * 16.f, 0.f } + pos, { 0.9f,0.1f,0.2f });
#endif // _DEBUG
				return true;
			}
		}
	}
	return false;
}

bool CollisionBodyComponent::CheckGrounded(float groundAngleBuffer)
{
	const float angle = 90.f;
	return CheckCollisionAtAngle(angle, groundAngleBuffer);
}

unsigned int CollisionBodyComponent::GetCollisionLayersMask()
{
	return m_body->GetFixtureList()->GetFilterData().maskBits;
}

std::set<std::string> CollisionBodyComponent::GetCollisionLayers()
{
	//TODO Implement
	return std::set<std::string>();
}

sol::table CollisionBodyComponent::LUAGetCollisionLayers()
{
	//TODO Implement
	return sol::table();
}

std::vector<b2Contact*> CollisionBodyComponent::GetContacts()
{
	std::vector<b2Contact*> contacts;
	for (b2ContactEdge* ce = m_body->GetContactList(); ce; ce = ce->next)
	{
		contacts.push_back(ce->contact);
	}
	return contacts;
}

bool CollisionBodyComponent::GetIsSensor()
{
	return m_body->GetFixtureList()->IsSensor();
}

void CollisionBodyComponent::SetIsSensor(bool isSensor)
{
	b2Fixture* fixture = m_body->GetFixtureList();

	while (fixture)
	{
		fixture->SetSensor(isSensor);
		fixture = fixture->GetNext();
	}
	//b2FixtureDef fixtureDef = CopyFixture(fixture);
	//m_body->CreateFixture(&fixtureDef);
	//m_body->DestroyFixture(fixture);
}


bool CollisionBodyComponent::IsBodySensor(b2Body* body)
{
	for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
		if (fixture->IsSensor()) {
			return true;
		}
	}
	return false;
}

b2FixtureDef CollisionBodyComponent::CopyFixture(b2Fixture* fixture)
{
	b2FixtureDef fixtureDef;
	fixtureDef.shape = fixture->GetShape();
	fixtureDef.friction = 0.f;
	fixtureDef.density = 1.f;
	fixtureDef.isSensor = fixture->IsSensor();
	fixtureDef.filter = fixture->GetFilterData();
	return fixtureDef;
}

void CollisionBodyComponent::LUABind(sol::state& lua)
{
	lua.new_usertype<CollisionBodyComponent>("collisionBodyComponent",
		"setVelocity", &CollisionBodyComponent::SetVelocity,
		"getVelocity", &CollisionBodyComponent::GetVelocity,
		"getPosition", &CollisionBodyComponent::GetPosition,
		"setPosition", &CollisionBodyComponent::SetPosition,
		"checkGrounded", &CollisionBodyComponent::CheckGrounded,
		"checkCollisionAtAngle", &CollisionBodyComponent::CheckCollisionAtAngle,
		"setIsSensor", &CollisionBodyComponent::SetIsSensor,
		"getIsSensor", &CollisionBodyComponent::GetIsSensor
	);
}