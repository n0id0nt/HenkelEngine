#include "SpriteComponent.h"

SpriteComponent::SpriteComponent(Entity* entity, Engine* engine, TileSheet tileSheet, unsigned int index) 
	: RenderComponent(entity, engine, 1u, tileSheet.GetTileSetImagePath()), m_tilesheet(tileSheet), m_index(index)
{
	m_Renderer.SetQuadUVs(m_tilesheet.GetSpriteRectAtIndex(m_index));
}

void SpriteComponent::Update(float deltaTime)
{
}
