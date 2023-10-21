#pragma once
#include "HenkelEngine.h"
#include "Resourse/TileSheet.h"
#include <vector>
#include "../RenderComponents/TileMapComponent.h"
#include "Physics\PhysicsWorld.h"

class TileMapCollisionBodyComponent 
{
public:
    TileMapCollisionBodyComponent(PhysicsWorld* world, b2FixtureDef fixtureDef, b2BodyDef bodyDef, const TileMapComponent& tilemap);
    ~TileMapCollisionBodyComponent();

private:

    std::vector<b2Body*> m_bodies;
    PhysicsWorld* m_world;
};

