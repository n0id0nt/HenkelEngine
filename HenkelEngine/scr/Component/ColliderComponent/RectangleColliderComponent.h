#pragma once
#include "ColliderComponent.h"

class RectangleColliderComponent :
    public ColliderComponent
{
public:
	RectangleColliderComponent(Entity* entity, float width, float height);

	void Update(float deltaTime) override;
private:

	b2PolygonShape m_collisionShape;
};

