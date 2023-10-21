#pragma once
#include "PhysicsBodyComponents\PhysicsBodyComponent.h"

class Engine;

class PlayerMovementComponent 
{
public:
    PlayerMovementComponent(Engine* engine, PhysicsBodyComponent* physicsBody);

    void Update(float deltaTime);

private:

    PhysicsBodyComponent* m_physicsBody;
    bool m_isGrounded = false;
    float m_verticalSpeed = 0.f;
    Engine* m_engine;
};

