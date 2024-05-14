#pragma once

#include <Box2D/Box2D.h>
#include <ECS\Entity\Entity.h>
#include <vector>
#include "glm\glm.hpp"

class ContactListener : public b2ContactListener {
public:
    struct Contact {
        Contact(Entity* other, b2Contact* contact, sol::state* lua);

        Entity* other;
        sol::table contactPoints;
        sol::table impulses;
        glm::vec2 velocity;
        glm::vec2 normal;
    };

    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact) override;

    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;

private:
    void GetContactEntities(b2Contact* contact, Entity*& entityA, Entity*& entityB);

};
