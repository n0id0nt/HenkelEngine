#pragma once

#include <vector>
#include "glm\glm.hpp"
#include "opengl\openglHelper.h"

constexpr int QUAD_SIZE = 4;

struct Vertex
{
	glm::vec3 pos;
	glm::vec4 color;
	glm::vec2 texCoord;
	float texIndex;
};

class Mesh
{
public:
	static void setCubeData(std::vector<Vertex>& vertices);
	static void setQuadData(std::vector<Vertex>& vertices, std::vector<GLuint>& indexes, unsigned int quadsCount);
	static void setQuadData(std::vector<GLuint>& indexes, unsigned int quadsCount);
	static void setQuadData(std::vector<Vertex>& vertices, const glm::vec2& pos, const glm::vec4& sourceRect, const bool& flipped = false, const glm::vec2& anchorPoint = glm::vec2(0.5f, 0.5f), const glm::vec4& color = glm::vec4(1.f,1.f,1.f,1.f), const glm::vec2& size = glm::vec2(1.0f, 1.0f), float textureIndex = 0.0f);
};

