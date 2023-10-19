#pragma once
#include "../Component.h"
#include "box2d.h"

class ColliderComponent :
    public Component
{
public:
    ColliderComponent(Entity* entity);

    virtual b2Shape* GetCollider() = 0;
};

