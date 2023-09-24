#pragma once

#include "Component.h"
#include "..\opengl\Shader.h"
#include "..\opengl\Texture.h"
#include "..\opengl\Mesh.h"
#include "glm\glm.hpp"
#include "../opengl/IRenderer.h"
#include "..\opengl\openglHelper.h"
#include <array>

class Panel;

struct Vertex
{
	glm::vec3 pos;
	glm::vec2 texCoord;
};

class RendererComponent : public Component, public IRenderer
{
public:
	RendererComponent(Entity* entity);
	void Update() {}

	void Render(Panel* panel) override;


private:

	void initOpenGL();

	Shader* m_Shader;
	Texture* m_Texture;
	std::vector<Vertex> m_Vertices;
	GLuint m_VAO, m_VBO;
};

