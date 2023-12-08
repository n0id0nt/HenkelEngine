#pragma once
#include "box2d\box2d.h"
#include "Physics\PhysicsWorld.h"

class StaticBodyComponent 
{
public:
    StaticBodyComponent(PhysicsWorld* world, glm::vec2 collisionShape);
    ~StaticBodyComponent();

    glm::vec2 GetPosition();
    void SetPosition(glm::vec2 pos);

    glm::vec2 GetCollisionShape();

private:

    b2Body* m_body;
    PhysicsWorld* m_world;
    glm::vec2 m_collisionShape;
};

