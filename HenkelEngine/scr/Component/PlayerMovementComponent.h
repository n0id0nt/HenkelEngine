#pragma once
#include "Component.h"
class PlayerMovementComponent :
    public Component
{
public:
    PlayerMovementComponent(Entity* entity);

    void Update(float deltaTime) override;
};

