#include "TileMapComponent.h"

TileMapComponent::TileMapComponent(Entity* entity, unsigned int width, unsigned int height, std::vector<unsigned> levelArray, TileSheet tileSheet)
	: RenderComponent(entity, width * height, tileSheet.GetTileSetImagePath()), m_Width(width), m_Height(height), m_LevelArray(levelArray), m_tileSheet(tileSheet)
{
	ASSERT(width * height == levelArray.size());
}

void TileMapComponent::Update(float deltaTime)
{
	
}

void TileMapComponent::Render()
{
	// determine if what aspects of the tilemap to draw
	for (unsigned int row = 0; row < m_Height; row++)
	{
		for (unsigned int col = 0; col < m_Width; col++)
		{
			unsigned int tile = GetTile(row, col);
			if (tile != 0)
				m_Renderer.AddQuadToBatch({ col, row }, m_tileSheet.GetSpriteRectAtIndex(tile - 1));
		}
	}

	RenderComponent::Render();
}

unsigned int TileMapComponent::GetTile(unsigned int row, unsigned int col)
{
	return m_LevelArray[row * m_Width + col];
}
