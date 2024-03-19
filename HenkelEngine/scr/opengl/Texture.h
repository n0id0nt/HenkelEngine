#pragma once

#include <string>

class Texture
{
private:
	unsigned int m_RendererID;
	int m_Width, m_Height;

	void CreateTexture(unsigned char* textureBuffer);

public:
	Texture(const std::string& path);
	Texture();
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }

};

