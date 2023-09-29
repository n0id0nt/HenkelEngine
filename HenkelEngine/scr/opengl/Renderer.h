#pragma once

#include "..\opengl\Shader.h"
#include "..\opengl\Texture.h"
#include "glm\glm.hpp"
#include "..\opengl\openglHelper.h"
#include <array>
#include <memory>

struct Vertex
{
	glm::vec3 pos;
	glm::vec2 texCoord;
};

class Renderer
{
public:
	Renderer(std::string vertexShader, std::string fragmentShader, std::string texture);

	void Render(glm::mat4 model, glm::mat4 view, glm::mat4 projection);

	Texture* GetTexture();

private:

	void initRenderData();

	std::unique_ptr<Shader> m_Shader;
	std::unique_ptr<Texture> m_Texture;
	std::vector<Vertex> m_Vertices;
	GLuint m_VAO;
};

