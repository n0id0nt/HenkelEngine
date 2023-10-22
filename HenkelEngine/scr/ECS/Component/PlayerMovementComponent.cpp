#include "PlayerMovementComponent.h"
#include "Input.h"
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

PlayerMovementComponent::PlayerMovementComponent(Engine* engine, PhysicsBodyComponent* physicsBody) 
	: m_engine(engine), m_physicsBody(physicsBody)
{
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
		m_isGrounded = m_physicsBody->CheckGrounded(40.f);

		if (!m_isGrounded)
		{
			m_verticalSpeed += gravity * deltaTime;

			//if (m_verticalSpeed < fallSpeed) m_verticalSpeed = fallSpeed;
		}
	}

	// jump 
	{
		if (m_engine->GetInput()->isInputDown("Jump") && m_isGrounded)
		{
			m_verticalSpeed = -jumpSpeed;
		}
	}

	m_physicsBody->SetVelocity({ horrizontalSpeed, m_verticalSpeed });
}
