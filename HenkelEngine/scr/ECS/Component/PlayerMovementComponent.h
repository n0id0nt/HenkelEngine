#pragma once
#include "Component.h"
#include "PhysicsBodyComponents\PhysicsBodyComponent.h"

class Engine;

class PlayerMovementComponent :
    public Component
{
public:
    PlayerMovementComponent(Entity* entity, Engine* engine);

    void Update(float deltaTime) override;

    bool CheckGrounded();

private:

    PhysicsBodyComponent* m_physicsBody;
    bool m_isGrounded = false;
    float m_verticalSpeed = 0.f;
    Engine* m_engine;
};

