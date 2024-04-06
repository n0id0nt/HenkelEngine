#pragma once

#include "glm\glm.hpp"
#include "opengl/openglHelper.h"
#include "ECS/Component/RenderComponents/MaterialComponent.h"
#include <array>
#include <memory>
#include "opengl/Mesh/Mesh.h"
#include "opengl/Texture.h"

class BatchRenderer
{
public:
	BatchRenderer(unsigned int quads = 1);
	~BatchRenderer();

	void Render();

	void LoadTexture(const std::string& texture);
	void UnloadTexture(const std::string& texture);

	void BindTextures();
	void UnbindTextures();

	void AddTextureToBatch(const std::string& texture, const glm::vec2& pos, const glm::vec2& uvPos, const glm::vec2& uvSize, const glm::vec2& anchorPoint = glm::vec2(0.5f, 0.5f), const glm::vec4& color = glm::vec4(1.f, 1.f, 1.f, 1.f), const glm::vec2& size = glm::vec2(1.0f, 1.0f));
	void AddQuadToBatch(const glm::vec2& pos, const glm::vec2& uvPos, const glm::vec2& uvSize, const glm::vec2& anchorPoint = glm::vec2(0.5f, 0.5f), const glm::vec4& color = glm::vec4(1.f, 1.f, 1.f, 1.f), const glm::vec2& size = glm::vec2(1.0f, 1.0f), int textureIndex = 0);

	void SetQuadUVs(const glm::vec4& rect, const bool& flipped);

	bool BatchValid();
	void InvalidateBatch();
	void ValidateBatch();
	void ClearBatch();

private:
	void InitRenderData();

	int GetTextureIndex(const std::string& texture);

	bool IsBatching();
	bool m_batchValid;

	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indexes;

	std::vector<std::string> m_textures;

	static constexpr const int INVALID_TEXTURE = -1; //TODO maybe move this???

	GLuint m_VAO, m_VBO, m_IBO;

	unsigned int m_quads; // the size of the batch
};

