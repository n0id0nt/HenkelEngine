#pragma once

#include "glm\glm.hpp"
#include "box2d\box2d.h"
#include "Resourse/TileSheet.h"
#include <vector>
#include <unordered_set>
#include <array>
#include "../RenderComponents/TileMapComponent.h"
#include "Physics\PhysicsWorld.h"
#include <ECS\Registry.h>
#include <ECS\Entity\Entity.h>
#include "CollisionBodyComponent.h"

class TileMapCollisionBodyComponent : public CollisionBodyComponent
{
public:
    TileMapCollisionBodyComponent(PhysicsWorld* world, const TileMapComponent& tilemap, Entity* entity, bool isSensor, uint16 categoryBits, uint16 maskBits);

    static void LUABind(sol::state& lua);

private:

    enum class Dir {
        Right,
        Down,
        Left,
        Up,

        Count
    };

    // Custom hash function for glm::ivec2
    struct IVec2Hash
    {
        std::size_t operator()(const glm::ivec2& v) const {
            // Combine hash values of each component
            std::hash<unsigned int> hasher;
            std::size_t hash = hasher(v.x);
            hash ^= hasher(v.y) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            return hash;
        }
    };

    bool IsTileClearAtDir(glm::ivec2 tile, const TileMapComponent& tilemap, Dir inputDir);
    Dir NextDir(Dir inputDir);
    Dir PreviousDir(Dir inputDir);
    bool IsNextDirClear(glm::ivec2 tile, const TileMapComponent& tilemap, Dir inputDir);
    glm::ivec2 GetTileInDir(glm::ivec2 tile, Dir inputDir);
    void CreateLoop(Dir inputDir, glm::ivec2 inputTile, std::unordered_set<glm::ivec2, IVec2Hash>& checkedTiles, const TileMapComponent& tilemap, bool isSensor, const b2Filter& filter);
    std::array<b2Vec2, 2> GetSideLine(Dir inputDir, glm::ivec2 inputTile, const TileMapComponent& tilemap);
};

