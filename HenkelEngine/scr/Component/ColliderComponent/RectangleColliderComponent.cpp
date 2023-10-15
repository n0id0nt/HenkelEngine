#include "RectangleColliderComponent.h"

RectangleColliderComponent::RectangleColliderComponent(Entity* entity, float width, float height) : ColliderComponent(entity)
{
	m_collisionShape.SetAsBox(width, height);
}

void RectangleColliderComponent::Update(float deltaTime)
{
}
