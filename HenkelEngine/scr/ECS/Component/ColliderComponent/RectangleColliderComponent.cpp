#include "RectangleColliderComponent.h"
#include "..\..\opengl\DebugRenderer.h"
#include "../../Entity/Entity.h"

RectangleColliderComponent::RectangleColliderComponent(Entity* entity, float width, float height) 
	: ColliderComponent(entity), m_width(width), m_height(height)
{
	m_collisionShape.SetAsBox(width/2.f, height/2.f);
}

void RectangleColliderComponent::Update(float deltaTime)
{
	DebugRenderer::DrawRectangle(GetEntity()->GetTransform()->GetWorldPosition(), m_width, m_height);
}

b2Shape* RectangleColliderComponent::GetCollider()
{
	return &m_collisionShape;
}
