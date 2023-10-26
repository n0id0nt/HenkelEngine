#include "TileSheet.h"

#include "pugixml.hpp"
#include "..\opengl\openglHelper.h"

TileSheet::TileSheet(std::string dir, std::string tileMapFile, int indexOffset) : m_dir(dir), m_indexOffset(indexOffset)
{
	LoadFile(dir + tileMapFile);
}

void TileSheet::LoadFile(std::string tileMapFile)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(tileMapFile.c_str());
	ASSERT(result);

	// level information to know how to read the file
	m_name = doc.child("tileset").attribute("name").as_string();
	m_tileWidth = doc.child("tileset").attribute("tilewidth").as_uint();
	m_tileHeight = doc.child("tileset").attribute("tileheight").as_uint();
	m_tileCount = doc.child("tileset").attribute("tilecount").as_uint();
	m_columns = doc.child("tileset").attribute("columns").as_uint();

	for (auto& image : doc.child("tileset").children("image"))
	{
		m_imageFile = image.attribute("source").as_string();
		m_imageWidth = image.attribute("width").as_uint();
		m_imageHeight = image.attribute("height").as_uint();
	}
}


glm::vec2 TileSheet::GetSpriteAtIndex(unsigned int index)
{
	return glm::vec2();
}

glm::vec4 TileSheet::GetSpriteRectAtIndex(unsigned int index)
{
	int x = ((index - m_indexOffset) % m_columns) * m_tileWidth;
	int y = glm::floor(index / m_columns) * m_tileHeight;
	float perX = x / (float)m_imageWidth;
	float perY = y / (float)m_imageHeight;
	float perWidth = m_tileWidth / (float)m_imageWidth + perX;
	float perHeight = m_tileHeight / (float)m_imageHeight + perY;
	return glm::vec4({perX, perY, perWidth, perHeight});
}

std::string TileSheet::GetTileSetImagePath() const
{
	return m_dir + m_imageFile;
}

unsigned int TileSheet::GetTileWidth() const
{
	return m_tileWidth;
}

unsigned int TileSheet::GetTileHeight() const
{
	return m_tileHeight;
}
