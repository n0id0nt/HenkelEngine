#pragma once

#include "glm\glm.hpp"
#include "box2d\box2d.h"
#include "glm\glm.hpp"
#include <vector>
#include "Physics\PhysicsWorld.h"
#include <sol\sol.hpp>
#include <ECS\Registry\Registry.h>
#include <ECS\Entity\Entity.h>
#include "CollisionBodyComponent.h"
#include <set>

class PhysicsBodyComponent : public CollisionBodyComponent
{
public:
    PhysicsBodyComponent(PhysicsWorld* world, glm::vec2 collisionShape, Entity* entity, bool isSensor, uint16 categoryBits, uint16 maskBits);
    PhysicsBodyComponent(const PhysicsBodyComponent&) = delete;

    static void LUABind(sol::state& lua);
};

