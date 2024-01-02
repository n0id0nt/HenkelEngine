#pragma once
#include "box2d\box2d.h"
#include "Physics\PhysicsWorld.h"
#include <ECS\Registry.h>
#include <ECS\Entity\Entity.h>

class StaticBodyComponent 
{
public:
    StaticBodyComponent(PhysicsWorld* world, glm::vec2 collisionShape, Entity* entity, bool isSensor, uint16 categoryBits, uint16 maskBits);
    ~StaticBodyComponent();

    glm::vec2 GetPosition();
    void SetPosition(glm::vec2 pos);

    glm::vec2 GetCollisionShape();

private:

    b2Body* m_body;
    PhysicsWorld* m_world;
    glm::vec2 m_collisionShape;
};

