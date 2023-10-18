#pragma once
#include "../Component.h"
#include "box2d.h"
#include "../../Resourse/TileSheet.h"
#include <vector>
#include "../RenderComponents/TileMapComponent.h"

class TileMapCollisionBodyComponent :
    public Component
{
public:
    TileMapCollisionBodyComponent(Entity* entity, b2World* world);
    ~TileMapCollisionBodyComponent();

    void Update(float deltaTime) override;

private:

    std::vector<b2Body*> m_bodies;
    b2World* m_world;
    TileMapComponent* m_tilemap;
};

