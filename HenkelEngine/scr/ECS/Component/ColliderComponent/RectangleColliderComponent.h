#pragma once
#include "ColliderComponent.h"

class RectangleColliderComponent :
    public ColliderComponent
{
public:
	RectangleColliderComponent(Entity* entity, float width, float height);

	void Update(float deltaTime) override;

	b2Shape* GetCollider() override;
private:

	b2PolygonShape m_collisionShape;

	float m_width, m_height;
};

