#pragma once
#include "box2d\box2d.h"
#include "Physics\PhysicsWorld.h"
#include <ECS\Registry.h>
#include <ECS\Entity\Entity.h>
#include <sol\sol.hpp>
#include "CollisionBodyComponent.h"

class StaticBodyComponent : public CollisionBodyComponent
{
public:
    StaticBodyComponent(PhysicsWorld* world, glm::vec2 collisionShape, Entity* entity, bool isSensor, uint16 categoryBits, uint16 maskBits);
    StaticBodyComponent(const StaticBodyComponent&) = delete;

    static void LUABind(sol::state& lua);
};

