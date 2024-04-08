#pragma once

#include <opengl\BatchRenderer.h>
#include <map>

class Font
{
public:
	Font(const std::string& font, int size, unsigned int& renderID, int& width, int& height);
	~Font();

	void RenderFont(BatchRenderer* batchRenderer, const std::string& text, float x, float y, const glm::vec4& color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

private:

	void CreateFont(const std::string& font, int size, unsigned int& renderID, int& width, int& height);

	struct Character {
		glm::ivec2 size;       // Size of glyph
		glm::ivec2 bearing;    // Offset from baseline to left/top of glyph
		glm::ivec2 advance;    // Offset to advance to next glyph
		float	   textureX;   // x offset of glyph in texture coordinates
	};

	Character m_characters[128];

	std::string m_font;
	int m_size;
	glm::vec2 m_atlasSize;
};

