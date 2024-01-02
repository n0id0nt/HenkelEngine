#pragma once
#include "HenkelEngine.h"
#include "glm\glm.hpp"
#include <vector>
#include "Physics\PhysicsWorld.h"
#include <sol\sol.hpp>
#include <ECS\Registry.h>
#include <ECS\Entity\Entity.h>

class PhysicsBodyComponent
{
public:
    PhysicsBodyComponent(PhysicsWorld* world, glm::vec2 collisionShape, Entity* entity, bool isSensor);
    PhysicsBodyComponent(const PhysicsBodyComponent&) = delete;
    ~PhysicsBodyComponent();
    
    void SetVelocity(glm::vec2 velocity);
    glm::vec2 GetVelocity();

    glm::vec2 GetPosition();
    void SetPosition(glm::vec2 pos);

    glm::vec2 GetCollisionShape();

    bool CheckCollisionAtAngle(float angle, float groundAngleBuffer);
    bool CheckGrounded(float groundAngleBuffer);

    static void LUABind(sol::state& lua);

    std::vector<b2Contact*> GetContacts();

private:

    b2Body* m_body;
    PhysicsWorld* m_world;
    glm::vec2 m_collisionShape;

};

