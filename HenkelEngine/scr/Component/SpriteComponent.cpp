#include "SpriteComponent.h"

SpriteComponent::SpriteComponent(Entity* entity, TileSheet tileSheet, unsigned int index) : RenderComponent(entity, true), m_tilesheet(tileSheet), m_index(index)
{
	m_Renderer.SetQuadUVs(m_tilesheet.GetSpriteRectAtIndex(m_index));
}

void SpriteComponent::Update()
{
}
