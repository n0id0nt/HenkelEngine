#pragma once

#include "GL\glew.h"

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	void CreateBuffer(int width, int height);
	void DeleteBuffer();

	void Bind();
	void Unbind();

	GLuint GetTexture();

private:
	GLuint m_FBO;

	GLuint m_frameBufferTexture;
	GLuint m_depthId;

	int m_width, m_height;
};

