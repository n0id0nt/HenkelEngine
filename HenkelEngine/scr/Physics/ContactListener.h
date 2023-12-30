#pragma once

#include <Box2D/Box2D.h>
#include <ECS\Entity\Entity.h>

class ContactListener : public b2ContactListener {
public:
    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact) override;

private:
    void GetContactEntities(b2Contact* contact, Entity*& entityA, Entity*& entityB);
};
