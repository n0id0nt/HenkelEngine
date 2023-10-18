#pragma once
#include "../Component.h"
#include <box2d.h>
#include "glm\glm.hpp"
#include <vector>

class PhysicsBodyComponent :
    public Component
{
public:
    PhysicsBodyComponent(Entity* entity, b2World* world);
    ~PhysicsBodyComponent();

    void Update(float deltaTime) override {}

    void UpdatePos();
    
    void SetVelocity(glm::vec2 velocity);
    glm::vec2 GetVelocity();

    std::vector<b2Contact*> GetContacts();

private:

    b2Body* m_body;
    b2World* m_world;
};

