#include "PlayerMovementComponent.h"
#include "Input.h"
#include "ECS/Entity/Entity.h"
#include "opengl\openglHelper.h"
#include "Scene.h"
#include "opengl/DebugRenderer.h"
#include "glm/gtx/vector_angle.hpp"
#include "Engine.h"

const float speed = 80.f;
const float jumpSpeed = 100.f;
const float fallSpeed = 500.f;
const float gravity = 160.f;
const float groundAngle = 40;

PlayerMovementComponent::PlayerMovementComponent(Entity* entity, Engine* engine) : Component(entity), m_engine(engine)
{
	ASSERT(GetEntity()->HasComponent<PhysicsBodyComponent>());
	m_physicsBody = GetEntity()->GetComponent<PhysicsBodyComponent>();
}

void PlayerMovementComponent::Update(float deltaTime)
{
	m_verticalSpeed = m_physicsBody->GetVelocity().y;
	float horrizontalSpeed = 0.f;

	// horrizontal speed
	{
		float horrizontalDir = m_engine->GetInput()->GetArrowDir().x;
		horrizontalSpeed = horrizontalDir * speed;// *deltaTime;
	}
	
	// calculate gravity
	{
		m_isGrounded = CheckGrounded();

		if (!m_isGrounded)
		{
			m_verticalSpeed += gravity * deltaTime;

			//if (m_verticalSpeed < fallSpeed) m_verticalSpeed = fallSpeed;
		}
	}

	// jump 
	{
		if (m_engine->GetInput()->isKeyJustPressed(SDLK_SPACE) && m_isGrounded)
		{
			m_verticalSpeed = -jumpSpeed;
		}
	}

	m_physicsBody->SetVelocity({ horrizontalSpeed, m_verticalSpeed });
}

bool PlayerMovementComponent::CheckGrounded()
{
	for (auto& contact : m_physicsBody->GetContacts())
	{
		if (contact->IsTouching())
		{
			b2Vec2 normal = contact->GetManifold()->localNormal;
			//glm::vec3 pos = GetEntity()->GetTransform()->GetWorldPosition();
			//DebugRenderer::DrawLine(pos, glm::vec3{ normal.x * 8.f, normal.y * 8.f, 0.f } + pos, {1.f,0.f,0.f});
			bool isFixtureA = contact->GetFixtureA()->GetBody()->GetUserData().pointer == reinterpret_cast<uintptr_t>(GetEntity());
			float angle = glm::degrees(glm::angle(glm::vec2{ 0.f,1.f }, (isFixtureA ? 1.f : -1.f)* glm::vec2{ normal.x, normal.y }));
			if (angle < groundAngle)
			{
				return true;
			}
		}
	}
	return false;
}
