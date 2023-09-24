#include "FrameBuffer.h"

#include "SOIL2.h"
#include "openglHelper.h"
#include <iostream>

FrameBuffer::FrameBuffer() :
	m_FBO(), m_frameBufferTexture(), m_depthId(), m_width(), m_height()
{
	
}

FrameBuffer::~FrameBuffer()
{
    DeleteBuffer();
}

void FrameBuffer::CreateBuffer(int width, int height)
{
	m_width = width;
	m_height = height;

	GLCall(glGenFramebuffers(1, &m_FBO));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));

	// create the color attachment texture
	GLCall(glGenTextures(1, &m_frameBufferTexture));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_frameBufferTexture));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_frameBufferTexture, 0));

    GLCall(glGenRenderbuffers(1, &m_depthId));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_depthId));
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height));
    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthId));

    auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer Error: " << fboStatus << std::endl;

    Unbind();
}

void FrameBuffer::DeleteBuffer()
{
    if(m_FBO)
    {
        GLCall(glDeleteFramebuffers(GL_FRAMEBUFFER, &m_FBO));
        GLCall(glDeleteTextures(1, &m_frameBufferTexture));
        GLCall(glDeleteTextures(1, &m_depthId));
        m_frameBufferTexture = 0;
        m_depthId = 0;
    }
}

void FrameBuffer::Bind()
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));
    GLCall(glViewport(0, 0, m_width, m_height));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    GLCall(glEnable(GL_DEPTH_TEST));
}

void FrameBuffer::Unbind()
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

GLuint FrameBuffer::GetTexture()
{
    return m_frameBufferTexture;
}
