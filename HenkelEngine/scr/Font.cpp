#include "Font.h"
#include <map>
#include <format>
#include <ft2build.h>
#include FT_FREETYPE_H

Font::Font(const std::string& font, int size, unsigned int& renderID, int& width, int& height) : m_font(font), m_size(size)
{
	CreateFont(font, size, renderID, width, height);
}

Font::~Font()
{
}

void Font::RenderFont(BatchRenderer* batchRenderer, const std::string& text, float x, float y, const glm::vec4& color)
{
    std::string fontTexture = std::format("{} {}", m_font, m_size);
    batchRenderer->LoadTexture(fontTexture);
    std::string::const_iterator c;

    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = m_characters[*c];

        float xPos = x + ch.bearing.x;
        float yPos = y - (ch.size.y - ch.bearing.y);

        float xUV = ch.textureX;

        float w = ch.size.x;
        float h = ch.size.y;

        float wUV = w / m_atlasWidth + xUV;

        /* Skip glyphs that have no pixels */
        if (!w || !h)
            continue;

        batchRenderer->AddTextureToBatch(fontTexture, glm::vec2(xPos, yPos), glm::vec2(xUV, 0.0f), glm::vec2(wUV, 1.0f), glm::vec2(0, 1), color, glm::vec2(w, h));
        x += ch.advance.x;
    }
}

void Font::CreateFont(const std::string& font, int size, unsigned int& renderID, int& width, int& height)
{
    FT_Library ft;
    ASSERT(!FT_Init_FreeType(&ft));

    FT_Face face;
    ASSERT(!FT_New_Face(ft, font.c_str(), 0, &face));

    FT_Set_Pixel_Sizes(face, 0, size);

    FT_GlyphSlot glyph = face->glyph;
    unsigned int w = 0;
    unsigned int h = 0;

    for (int c = 32; c < 128; c++) {
        ASSERT(!FT_Load_Char(face, c, FT_LOAD_RENDER))

        w += glyph->bitmap.width;
        h = glm::max(h, glyph->bitmap.rows);
    }

    // generate texture
    GLCall(glGenTextures(1, &renderID));
    GLCall(glBindTexture(GL_TEXTURE_2D, renderID));
    // Set our texture parameters
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));
    // Set texture filtering
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    //GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1)); // disable byte-alignment restriction
    GLCall(glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        w,
        h,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        0
    ));

    int x = 0;
    for (int c = 32; c < 128; c++) {
        ASSERT(!FT_Load_Char(face, c, FT_LOAD_RENDER));

        GLubyte* expanded_data = new GLubyte[4 * glyph->bitmap.width * glyph->bitmap.rows];

        for (int j = 0; j < glyph->bitmap.rows; j++) {
            for (int i = 0; i < glyph->bitmap.width; i++) {
                expanded_data[4 * (i + j * glyph->bitmap.width)] =
                    expanded_data[4 * (i + j * glyph->bitmap.width) + 1] =
                    expanded_data[4 * (i + j * glyph->bitmap.width) + 2] = 255;
                expanded_data[4 * (i + j * glyph->bitmap.width) + 3] = glyph->bitmap.buffer[i + glyph->bitmap.width * j];
            }
        }

        GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, glyph->bitmap.width, glyph->bitmap.rows, GL_RGBA, GL_UNSIGNED_BYTE, expanded_data));

        x += glyph->bitmap.width;

        m_characters[c] = Character{
            glm::ivec2(glyph->bitmap.width, glyph->bitmap.rows),
            glm::ivec2(glyph->bitmap_left, glyph->bitmap_top),
            glm::ivec2(glyph->advance.x >> 6, glyph->advance.y >> 6),
            (float)x / w
        };
        
    }
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));

    GLCall(glBindTexture(GL_TEXTURE_2D, 0)); // Unbind texture when done, so we won't accidentily mess up our texture.

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    m_atlasWidth = w;
    width = w;
    height = h;
}
