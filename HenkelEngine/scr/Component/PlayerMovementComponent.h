#pragma once
#include "Component.h"
#include "PhysicsBodyComponents\PhysicsBodyComponent.h"

class PlayerMovementComponent :
    public Component
{
public:
    PlayerMovementComponent(Entity* entity);

    void Update(float deltaTime) override;

    bool CheckGrounded();

private:

    PhysicsBodyComponent* m_physicsBody;
    bool m_isGrounded = false;
    float m_verticalSpeed = 0.f;
};

