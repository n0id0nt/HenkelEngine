#pragma once
#include "../Component.h"
#include "box2d.h"
#include "Physics\PhysicsWorld.h"

class StaticBodyComponent :
    public Component
{
public:
    StaticBodyComponent(Entity* entity, PhysicsWorld* world);
    ~StaticBodyComponent();

    void Update(float deltaTime) override {}

private:

    b2Body* m_body;
    PhysicsWorld* m_world;

};

