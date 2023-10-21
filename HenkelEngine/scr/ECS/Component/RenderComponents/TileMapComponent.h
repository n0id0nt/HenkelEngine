#pragma once
#include "RenderComponent.h"
#include "Resourse/TileSheet.h"
#include <vector>
#include <array>

struct TileMapComponent
{
	unsigned int width, height;
	std::vector<unsigned> levelArray;
	TileSheet tileSheet;

	unsigned int GetTileWidth() const;
	unsigned int GetTileHeight() const;

	std::vector<glm::vec2> GetTilePositions() const;
	// TODO no check for camera pos need to ensure that the tile map is trimmed for the camera
	std::vector<std::array<glm::vec2, 3>> GetTileVertices();

	unsigned int GetTile(unsigned int row, unsigned int col) const;

};

