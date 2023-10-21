#pragma once

#include "glm\glm.hpp"
#include "opengl/openglHelper.h"
#include "opengl/Material.h"
#include <array>
#include <memory>
#include "opengl/Mesh/Mesh.h"

class RenderComponent
{
public:
	RenderComponent(Material* material, unsigned int quads = 1);
	~RenderComponent();

	void Render(glm::mat4 model, glm::mat4 view, glm::mat4 projection);

	void AddQuadToBatch(const glm::vec2& pos, const glm::vec2& uvPos, const glm::vec2& uvSize);

	void SetQuadUVs(const glm::vec4& rect);

private:
	void InitRenderData();

	bool IsBatching();

	std::vector<Vertex> m_Vertices;
	std::vector<GLuint> m_Indexes;
	std::unique_ptr<Material> m_Material;

	GLuint m_VAO, m_VBO, m_IBO; 

	unsigned int m_Quads; // the size of the batch

};

