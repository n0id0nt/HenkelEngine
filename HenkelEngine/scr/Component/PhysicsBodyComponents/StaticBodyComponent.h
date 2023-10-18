#pragma once
#include "../Component.h"
#include "box2d.h"

class StaticBodyComponent :
    public Component
{
public:
    StaticBodyComponent(Entity* entity, b2World* world);
    ~StaticBodyComponent();

    void Update(float deltaTime) override {}

private:

    b2Body* m_body;
    b2World* m_world;

};

