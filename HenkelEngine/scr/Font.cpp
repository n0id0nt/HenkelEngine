#include "Font.h"
#include <map>
#include <format>
#include <sstream>
#include <iostream>
#include <vector>
#include <ft2build.h>
#include FT_FREETYPE_H

Font::Font(const std::string& font, int size, unsigned int& renderID, int& width, int& height) 
    : m_font(font), m_size(size), m_atlasSize(), m_lineSpacing(size / 2.0f), m_wrappedLineSpacing(size / 10.0f)
{
	CreateFont(font, size, renderID, width, height);
}

Font::~Font()
{
}

void Font::RenderFont(BatchRenderer* batchRenderer, const std::string& text, float x, float y, float width, float height, TextHorizontalAlignment horizontalAlignment, TextWrapping wrapping, const glm::vec4& color)
{
    std::string fontTexture = std::format("{} {}", m_font, m_size);
    batchRenderer->LoadTexture(fontTexture);

    // get line width
    float curX = x;
    float curY = y + m_size;
    std::stringstream textStream(text);
    float spaceAdvance = m_characters[' '].advance.x;
    for (std::string curLine; std::getline(textStream, curLine, '\n');)
    {
        std::stringstream curLineStream(curLine);
        // calculate the how many words to put in the line 
        std::vector<std::string> wrappedLines{ std::string() };
        std::vector<float> wrappedLinesWidths{ 0.0f };
        for (std::string curWord; std::getline(curLineStream, curWord, ' ');)
        {
            float curWordWidth = GetStringDimentions(curWord).x;
            float curlineWidth = wrappedLinesWidths[wrappedLinesWidths.size() - 1] + curWordWidth;
            if (wrapping == TextWrapping::Wrap && curlineWidth > width)
            {
                wrappedLinesWidths[wrappedLinesWidths.size() - 1] -= spaceAdvance;
                wrappedLines[wrappedLines.size() - 1].resize(wrappedLines[wrappedLines.size() - 1].size() - 1); // take out extra space
                wrappedLinesWidths.push_back(0.0f);
                wrappedLines.push_back(std::string());
            }
            wrappedLinesWidths[wrappedLinesWidths.size() - 1] += curWordWidth + spaceAdvance;
            wrappedLines[wrappedLines.size() - 1].append(curWord);
            wrappedLines[wrappedLines.size() - 1].append(" ");
        }
        wrappedLinesWidths[wrappedLinesWidths.size() - 1] -= spaceAdvance;
        wrappedLines[wrappedLines.size() - 1].resize(wrappedLines[wrappedLines.size() - 1].size() - 1); // take out extra space

        // print the wrapped lines
        for (int i = 0; i < wrappedLines.size(); i++)
        {
            curX = x;
            switch (horizontalAlignment)
            {
            case TextHorizontalAlignment::Left:
                break;
            case TextHorizontalAlignment::Center:
                curX += (width - wrappedLinesWidths[i]) / 2.0f;
                break;
            case TextHorizontalAlignment::Right:
                curX += (width - wrappedLinesWidths[i]);
                break;
            default:
                ASSERT(false)
            }
            std::stringstream curWrappedLineStream(wrappedLines[i]);
            for (std::string curWord; std::getline(curWrappedLineStream, curWord, ' ');)
            {
                for (std::string::const_iterator c = curWord.begin(); c != curWord.end(); c++)
                {
                    ASSERT(*c >= 32 && *c < 128);
                    Character ch = m_characters[*c];

                    float xPos = curX + ch.bearing.x;
                    float yPos = curY + (ch.size.y - ch.bearing.y);

                    float xUV = ch.textureX;

                    float w = ch.size.x;
                    float h = ch.size.y;

                    float wUV = w / m_atlasSize.x;
                    float hUV = h / m_atlasSize.y;

                    curX += ch.advance.x;

                    /* Skip glyphs that have no pixels */
                    if (!w || !h)
                        continue;

                    batchRenderer->AddTextureToBatch(fontTexture, glm::vec2(xPos, yPos), glm::vec2(xUV - wUV, 0.0f), glm::vec2(xUV, hUV), glm::vec2(0, 1), color, glm::vec2(w, h));
                }
                curX += spaceAdvance;
            }
            curY += m_size + m_wrappedLineSpacing;
        }
        curY += m_lineSpacing - m_wrappedLineSpacing; // subracting wrapped line spacing because the previous line added the wrapped line spacing
    }
}

// TODO update this function to account for text wrapping
glm::vec2 Font::GetStringDimentions(const std::string& text)
{
    std::string::const_iterator c;
    float maxWidth = 0.0f, totalHeight = 0.0f;
    float curX = 0.0f, curY = 0.0f;
    float w = 0.0f, h = 0.0f;
    float advance = 0.0f;
    for (c = text.begin(); c != text.end(); c++)
    {
        if (*c == '\n')
        {
            float curLineWidth = curX;
            if (curLineWidth > maxWidth)
            {
                maxWidth = curLineWidth;
            }
            curX = 0.0f;
            curY += m_size + m_lineSpacing; // need better new line height 
            continue;
        }

        ASSERT(*c >= 32 && *c < 128);
        Character ch = m_characters[*c];

        w = ch.size.x;
        h = ch.size.y;
        advance = ch.advance.x;
        curX += advance;
    }

    totalHeight = curY;

    float curLineWidth = curX;
    if (curLineWidth > maxWidth)
    {
        maxWidth = curLineWidth;
    }

    return glm::vec2(maxWidth, totalHeight);
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

    width = w;
    height = h;
    m_atlasSize = glm::vec2(w, h);
}
