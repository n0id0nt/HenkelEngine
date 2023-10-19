#pragma once
#include "../Component.h"
#include "HenkelEngine.h"
#include "Resourse/TileSheet.h"
#include <vector>
#include "../RenderComponents/TileMapComponent.h"
#include "Physics\PhysicsWorld.h"

class TileMapCollisionBodyComponent :
    public Component
{
public:
    TileMapCollisionBodyComponent(Entity* entity, PhysicsWorld* world);
    ~TileMapCollisionBodyComponent();

    void Update(float deltaTime) override;

private:

    std::vector<b2Body*> m_bodies;
    PhysicsWorld* m_world;
    TileMapComponent* m_tilemap;
};

