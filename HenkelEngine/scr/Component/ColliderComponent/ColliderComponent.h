#pragma once
#include "../Component.h"
#include "box2d.h"

class ColliderComponent :
    public Component
{
public:
    ColliderComponent(Entity* entity);
};
