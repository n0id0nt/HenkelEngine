#pragma once
#include "HenkelEngine.h"
#include "glm\glm.hpp"
#include <vector>
#include "Physics\PhysicsWorld.h"

class PhysicsBodyComponent
{
public:
    PhysicsBodyComponent(PhysicsWorld* world, b2FixtureDef fixtureDef, b2BodyDef bodyDef);
    ~PhysicsBodyComponent();
    
    void SetVelocity(glm::vec2 velocity);
    glm::vec2 GetVelocity();

    glm::vec2 GetPosition();

    bool CheckGrounded(float groundAngle);

    std::vector<b2Contact*> GetContacts();

private:

    b2Body* m_body;
    PhysicsWorld* m_world;
};

