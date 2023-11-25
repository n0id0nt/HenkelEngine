#pragma once
#include "RenderComponent.h"
#include "Resourse/TileSheet.h"
#include <vector>
#include <array>

class TileMapComponent
{
public:
	TileMapComponent(unsigned int width, unsigned int height, std::vector<unsigned int> levelArray, TileSheet tileSheet);

	unsigned int GetWidth() const;
	unsigned int GetHeight() const;

	unsigned int GetTileWidth() const;
	unsigned int GetTileHeight() const;

	std::vector<glm::vec2> GetTilePositions() const;
	// TODO no check for camera pos need to ensure that the tile map is trimmed for the camera
	std::vector<std::array<glm::vec2, 3>> GetTileVertices();

	unsigned int GetTile(unsigned int row, unsigned int col) const;

private:
	unsigned int m_width, m_height;
	std::vector<unsigned int> m_levelArray;
	TileSheet m_tileSheet;
};

