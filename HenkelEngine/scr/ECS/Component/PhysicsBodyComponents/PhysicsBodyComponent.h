#pragma once
#include "ECS/Component/Component.h"
#include "HenkelEngine.h"
#include "glm\glm.hpp"
#include <vector>
#include "Physics\PhysicsWorld.h"

class PhysicsBodyComponent :
    public Component
{
public:
    PhysicsBodyComponent(Entity* entity, PhysicsWorld* world);
    ~PhysicsBodyComponent();

    void Update(float deltaTime) override {}

    void UpdatePos();
    
    void SetVelocity(glm::vec2 velocity);
    glm::vec2 GetVelocity();

    std::vector<b2Contact*> GetContacts();

private:

    b2Body* m_body;
    PhysicsWorld* m_world;
};

