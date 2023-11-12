#pragma once
#include "HenkelEngine.h"
#include "glm\glm.hpp"
#include <vector>
#include "Physics\PhysicsWorld.h"
#include <sol\sol.hpp>

class PhysicsBodyComponent
{
public:
    PhysicsBodyComponent(PhysicsWorld* world, glm::vec2 collisionShape);
    PhysicsBodyComponent(const PhysicsBodyComponent&) = delete;
    ~PhysicsBodyComponent();
    
    void SetVelocity(glm::vec2 velocity);
    glm::vec2 GetVelocity();

    glm::vec2 GetPosition();
    void SetPosition(glm::vec2 pos);

    glm::vec2 GetCollisionShape();

    bool CheckGrounded(float groundAngle);

    static void LUABind(sol::state& lua);

    std::vector<b2Contact*> GetContacts();

private:

    b2Body* m_body;
    PhysicsWorld* m_world;
    glm::vec2 m_collisionShape;

};

