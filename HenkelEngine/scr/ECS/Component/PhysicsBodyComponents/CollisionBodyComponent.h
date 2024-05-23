#pragma once
#include "glm\glm.hpp"
#include "box2d\box2d.h"
#include "glm\glm.hpp"
#include <vector>
#include "Physics\PhysicsWorld.h"
#include <sol\sol.hpp>
#include <ECS\Registry.h>
#include <ECS\Entity\Entity.h>
#include <set>

class CollisionBodyComponent
{
public:
    CollisionBodyComponent(PhysicsWorld* world);
    CollisionBodyComponent(const CollisionBodyComponent&) = delete;
    virtual ~CollisionBodyComponent();

    b2Body* GetBody() const;

    void SetVelocity(glm::vec2 velocity);
    glm::vec2 GetVelocity();

    glm::vec2 GetPosition();
    void SetPosition(glm::vec2 pos);

    // only used for debugging should eventually be changed to a more generic collision shape to draw more abstract fixtures
    // could take something from the tilemap collider to help draw more generic shapes???
    glm::vec2 GetCollisionShape();

    bool CheckCollisionAtAngle(float angle, float groundAngleBuffer);
    bool CheckGrounded(float groundAngleBuffer);

    unsigned int GetCollisionLayersMask();
    std::set<std::string> GetCollisionLayers();

    std::vector<b2Contact*> GetContacts();

    bool GetIsSensor();
    void SetIsSensor(bool isSensor);

    sol::table LUAGetCollisionLayers();

    static void LUABind(sol::state& lua);
    static bool IsBodySensor(b2Body* body);

protected:
    b2FixtureDef CopyFixture(b2Fixture* fixture);

    b2Body* m_body;
    PhysicsWorld* m_world;
    glm::vec2 m_collisionShape;

};

