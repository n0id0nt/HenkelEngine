#pragma once

#include "glm\glm.hpp"
#include "..\opengl\openglHelper.h"
#include "Material.h"
#include <array>
#include <memory>

struct Vertex
{
	glm::vec3 pos;
	glm::vec4 color;
	glm::vec2 texCoord;
};

class Renderer
{
public:
	Renderer(Material* material, unsigned int quads = 1);

	void Render(glm::mat4 model, glm::mat4 view, glm::mat4 projection);

	void AddQuadToBatch(const glm::vec2& pos, const glm::vec4& size);

	void SetQuadUVs(const glm::vec4& rect);

private:
	void InitRenderData();

	bool IsBatching();

	std::vector<Vertex> m_Vertices;
	std::vector<GLuint> m_Indexes;
	std::unique_ptr<Material> m_Material;

	GLuint m_VAO, m_VBO; 

	unsigned int m_Quads; // the size of the batch

};

