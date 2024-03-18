#pragma once

#include "glm\glm.hpp"
#include "opengl/openglHelper.h"
#include "ECS/Component/RenderComponents/MaterialComponent.h"
#include <array>
#include <memory>
#include "opengl/Mesh/Mesh.h"
#include "opengl/BatchRenderer.h"

class RenderComponent
{
public:
	RenderComponent(unsigned int quads = 1);
	~RenderComponent();

	void Render();

	void AddQuadToBatch(const glm::vec2& pos, const glm::vec2& uvPos, const glm::vec2& uvSize);

	void SetQuadUVs(const glm::vec4& rect, const bool& flipped);

	bool BatchValid();
	void InvalidateBatch();
	void ValidateBatch();
	void ClearBatch();

	BatchRenderer* GetBatchRenderer();

private:
	BatchRenderer m_batchRenderer;

};

