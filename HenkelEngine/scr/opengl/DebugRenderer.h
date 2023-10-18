#pragma once
#include "glm\common.hpp"
#include <glm\fwd.hpp>
#include <vector>
#include "Shader.h"
#include "openglHelper.h"


struct DebugVertex
{
	glm::vec3 pos;
	glm::vec3 color;
};

class DebugRenderer
{
public:
	static void Render(glm::mat4 viewProjection);
	static void InitDebugRenderer();
	static void ExitDebugRenderer();

	static void DrawLine(glm::vec3 point1, glm::vec3 point2, glm::vec3 color = {0.f,1.f,0.f});
	static void DrawRectangle(glm::vec3 position, float width, float height, glm::vec3 color = { 0.f,1.f,0.f });
	static void DrawCircle(glm::vec3 position, float radius, glm::vec3 color = { 0.f,1.f,0.f });

private:

	static GLuint s_VAO, s_VBO;
	static std::vector<DebugVertex> s_Vertices;
	static Shader* s_shader;
};

