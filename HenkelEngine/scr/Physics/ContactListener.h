#pragma once

#include <Box2D/Box2D.h>
#include <ECS\Entity\Entity.h>
#include <vector>

class ContactListener : public b2ContactListener {
public:
    struct Contact {
        Entity* other;
        sol::table contactPoints;
        float impulse;
        float velocity;
    };

    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact) override;

private:
    void GetContactEntities(b2Contact* contact, Entity*& entityA, Entity*& entityB);

};
