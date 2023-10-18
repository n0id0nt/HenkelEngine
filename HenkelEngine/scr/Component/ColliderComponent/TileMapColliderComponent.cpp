#include "TileMapColliderComponent.h"

TileMapColliderComponent::TileMapColliderComponent(Entity* entity) : ColliderComponent(entity)
{
}

void TileMapColliderComponent::Update(float deltaTime)
{
}

b2Shape* TileMapColliderComponent::GetCollider()
{
	return nullptr;
}
