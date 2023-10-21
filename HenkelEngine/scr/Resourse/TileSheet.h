#pragma once

#include <string>
#include "glm\glm.hpp"

class TileSheet
{
public:
	TileSheet(std::string dir, std::string tileMapFile, int indexOffset = 0);
	
	//void LoadTileSheet(std::string name, std::string imageFile, unsigned int tileWidth, unsigned int tileHeight, unsigned int tileCount, unsigned int columns, unsigned int imageWidth, unsigned int imageHeight, unsigned int paddingX = 0, unsigned int paddingY = 0, unsigned int borderX = 0, unsigned int borderY = 0);

	glm::vec2 GetSpriteAtIndex(unsigned int index);
	glm::vec4 GetSpriteRectAtIndex(unsigned int index);

	std::string GetTileSetImagePath();

	unsigned int GetTileWidth() const;
	unsigned int GetTileHeight() const;

private:

	void LoadFile(std::string tileMapFile);

	unsigned int m_tileWidth, m_tileHeight;
	unsigned int m_tileCount;
	unsigned int m_columns;
	unsigned int m_imageWidth, m_imageHeight;
	unsigned int m_borderX, m_borderY;
	unsigned int m_paddingX, m_paddingY;
	// todo change the file to a texture or a reference to the texture
	std::string m_imageFile;
	std::string m_dir;
	std::string m_name;

	int m_indexOffset;
};

