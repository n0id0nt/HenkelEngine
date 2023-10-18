#pragma once
#include "RenderComponent.h"
#include "../../Resourse/TileSheet.h"
#include <vector>

class TileMapComponent : public RenderComponent
{
public:
	TileMapComponent(Entity* entity, unsigned int width, unsigned int height, std::vector<unsigned> levelArray, TileSheet tileSheet);

	void Update(float deltaTime) override;
	void Render() override;

	unsigned int GetTileWidth();
	unsigned int GetTileHeight();

	std::vector<glm::vec2> GetTilePositions();

private:
	unsigned int GetTile(unsigned int row, unsigned int col);

	unsigned int m_Width, m_Height;
	std::vector<unsigned> m_LevelArray;
	TileSheet m_tileSheet;
};

