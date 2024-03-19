#include "Texture.h"

#include "SOIL2/SOIL2.h"
#include "GL\glew.h"
#include "openglHelper.h"

void Texture::CreateTexture(unsigned char* textureBuffer)
{
    // --== TEXTURE == --
    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID)); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
    // Set our texture parameters
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));
    // Set texture filtering
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    // Load, create texture and generate mipmaps
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureBuffer));
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));

    GLCall(glBindTexture(GL_TEXTURE_2D, 0)); // Unbind texture when done, so we won't accidentily mess up our texture.
}

Texture::Texture(const std::string& path)
	: m_RendererID(0), m_FilePath(path), m_Width(0), m_Height(0)
{   
    unsigned char* localBuffer = SOIL_load_image(path.c_str(), &m_Width, &m_Height, 0, SOIL_LOAD_RGBA);
    CreateTexture(localBuffer);
    SOIL_free_image_data(localBuffer);
}

Texture::Texture()
    : m_RendererID(0), m_FilePath("blank"), m_Width(1), m_Height(1)
{
    unsigned char localBuffer[] = {255, 255, 255, 255}; // Single White Pixel
    CreateTexture(localBuffer);
}

Texture::~Texture()
{
    GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
