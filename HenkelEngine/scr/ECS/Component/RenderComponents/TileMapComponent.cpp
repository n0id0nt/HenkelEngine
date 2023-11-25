#include "TileMapComponent.h"
#include <bitset>
#include <iostream>

const unsigned int horizontal_flip_bit_mask = 1u << 31u;
const unsigned int vertical_flip_bit_mask = 1u << 30u;

TileMapComponent::TileMapComponent(unsigned int width, unsigned int height, std::vector<unsigned int> levelArray, TileSheet tileSheet)
	: m_width(width), m_height(height), m_levelArray(levelArray), m_tileSheet(tileSheet)
{
}

unsigned int TileMapComponent::GetWidth() const
{
	return m_width;
}

unsigned int TileMapComponent::GetHeight() const
{
	return m_height;
}

unsigned int TileMapComponent::GetTileWidth() const
{
	return m_tileSheet.GetTileWidth();
}

unsigned int TileMapComponent::GetTileHeight() const
{
	return m_tileSheet.GetTileHeight();
}

std::vector<glm::vec2> TileMapComponent::GetTilePositions() const
{
	std::vector<glm::vec2> tilePositions;
	for (unsigned int row = 0; row < m_height; row++)
	{
		for (unsigned int col = 0; col < m_width; col++)
		{
			unsigned int tile = GetTile(row, col);
			if (tile != 0)
				tilePositions.push_back({ col * GetTileWidth(), row * GetTileHeight() });
		}
	}
	return tilePositions;
}

std::vector<std::array<glm::vec2, 3>> TileMapComponent::GetTileVertices()
{
	std::vector<std::array<glm::vec2, 3>> vertices;
	// determine if what aspects of the tilemap to draw
	for (unsigned int row = 0; row < m_height; row++)
	{
		for (unsigned int col = 0; col < m_width; col++)
		{
			unsigned int tile = GetTile(row, col);
			if (tile != 0)
			{
				bool flipped = tile & horizontal_flip_bit_mask;
				if (flipped)
				{
					tile ^= horizontal_flip_bit_mask;
				}
				glm::vec4 uvs = m_tileSheet.GetSpriteRectAtIndex(tile - 1, flipped);
				vertices.push_back(std::array<glm::vec2, 3>{ glm::vec2{ col, row },  { uvs.x, uvs.y }, { uvs.z, uvs.w } });
			}
		}
	}
	return vertices;
}

unsigned int TileMapComponent::GetTile(unsigned int row, unsigned int col) const
{
	return m_levelArray[row * m_width + col];
}
