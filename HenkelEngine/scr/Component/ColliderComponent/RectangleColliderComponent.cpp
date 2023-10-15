#include "RectangleColliderComponent.h"
#include "..\..\opengl\DebugRenderer.h"
#include "../../Entity/Entity.h"

RectangleColliderComponent::RectangleColliderComponent(Entity* entity, float width, float height) 
	: ColliderComponent(entity), m_width(width), m_height(height)
{
	m_collisionShape.SetAsBox(width, height);
}

void RectangleColliderComponent::Update(float deltaTime)
{
	DebugRenderer::DrawRectangle(GetEntity()->GetTransform()->GetPosition(), m_width, m_height);
}
