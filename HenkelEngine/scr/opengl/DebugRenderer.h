#pragma once
#include "glm\common.hpp"
#include <glm\fwd.hpp>
#include <vector>

class DebugRenderer
{
public:
	static void Render();

	static void DrawPoint(glm::vec3 position);
	static void DrawLine(glm::vec3 point1, glm::vec3 point2);
	static void DrawRectangle(glm::vec3 position, float width, float height);
	static void DrawCircle(glm::vec3 position, float radius);

private:
	static std::vector<glm::vec3> s_points;
	static std::vector<glm::vec3> s_lineP1;
	static std::vector<glm::vec3> s_lineP2;
	static std::vector<glm::vec3> s_cirP;
	static std::vector<float> s_cirR;

};

