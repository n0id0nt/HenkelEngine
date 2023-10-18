#pragma once
#include "ColliderComponent.h"
class TileMapColliderComponent :
    public ColliderComponent
{
public:
	TileMapColliderComponent(Entity* entity);

	void Update(float deltaTime) override;

	b2Shape* GetCollider() override;
private:

	b2PolygonShape m_collisionShape;

	float m_width, m_height;
};

