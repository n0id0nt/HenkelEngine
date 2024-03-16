#pragma once

#include "glm\glm.hpp"
#include "opengl/openglHelper.h"
#include "ECS/Component/RenderComponents/MaterialComponent.h"
#include <array>
#include <memory>
#include "opengl/Mesh/Mesh.h"

class BatchRenderer
{
public:
	BatchRenderer(unsigned int quads = 1);
	~BatchRenderer();

	void Render();

	void AddQuadToBatch(const glm::vec2& pos, const glm::vec2& uvPos, const glm::vec2& uvSize);

	void SetQuadUVs(const glm::vec4& rect, const bool& flipped);

	bool BatchValid();
	void InvalidateBatch();
	void ValidateBatch();
	void ClearBatch();

private:
	void InitRenderData();

	bool IsBatching();
	bool m_batchValid;

	std::vector<Vertex> m_Vertices;
	std::vector<GLuint> m_Indexes;

	GLuint m_VAO, m_VBO, m_IBO;

	unsigned int m_Quads; // the size of the batch
};

