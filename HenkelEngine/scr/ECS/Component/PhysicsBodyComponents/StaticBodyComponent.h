#pragma once
#include "box2d.h"
#include "Physics\PhysicsWorld.h"

class StaticBodyComponent 
{
public:
    StaticBodyComponent(PhysicsWorld* world, b2FixtureDef fixtureDef, b2BodyDef bodyDef);
    ~StaticBodyComponent();

    glm::vec2 GetPosition();

private:

    b2Body* m_body;
    PhysicsWorld* m_world;

};

