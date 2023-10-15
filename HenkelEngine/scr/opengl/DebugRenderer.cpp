#include "DebugRenderer.h"
#include <GL\glew.h> 
#include "openglHelper.h"
#include <glm\ext\scalar_constants.hpp>
#include <glm\trigonometric.hpp>

std::vector<glm::vec3> DebugRenderer::s_points;
std::vector<glm::vec3> DebugRenderer::s_lineP1;
std::vector<glm::vec3> DebugRenderer::s_lineP2;
std::vector<glm::vec3> DebugRenderer::s_cirP;
std::vector<float> DebugRenderer::s_cirR;

void DebugRenderer::Render()
{
	GLCall(glOrtho(-100.f, 100.f, -100.f, 100.f, -100.f, 100.f));
	// draw points
	{
		GLCall(glBegin(GL_POINTS));

		for (auto& point : s_points)
		{
			GLCall(glVertex3f(point.x, point.y, point.z));
		}

		GLCall(glEnd());
	}
	s_points.clear();

	// draw line
	{
		ASSERT(s_lineP1.size() == s_lineP2.size());

		GLCall(glBegin(GL_POINTS));
		for (int i = 0; i < s_lineP1.size(); i++)
		{
			GLCall(glVertex3f(s_lineP1[i].x, s_lineP1[i].y, s_lineP1[i].z));
			GLCall(glVertex3f(s_lineP2[i].x, s_lineP2[i].y, s_lineP2[i].z));
		}
		GLCall(glEnd());
	}
	s_lineP1.clear();
	s_lineP2.clear();

	// draw circle
	{
		ASSERT(s_cirP.size() == s_cirR.size());
		GLfloat x, y, angle;
		GLCall(glBegin(GL_POINTS));  
		for (int i = 0; i < s_cirP.size(); i++)
		{
			for (angle = 0.0f; angle <= (2.0f * glm::pi<float>()); angle += 0.01f)
			{
				x = s_cirR[i] * glm::sin(angle);
				y = s_cirR[i] * glm::cos(angle);
				GLCall(glVertex3f(s_cirP[i].x, s_cirP[i].y, s_cirP[i].z));
			}
		}
		GLCall(glEnd());
	}
	s_cirP.clear();
	s_cirR.clear();
}

void DebugRenderer::DrawPoint(glm::vec3 position)
{
	s_points.push_back(position);
}

void DebugRenderer::DrawLine(glm::vec3 point1, glm::vec3 point2)
{
	s_lineP1.push_back(point1);
	s_lineP2.push_back(point2);
}

void DebugRenderer::DrawRectangle(glm::vec3 position, float width, float height)
{
	DrawLine({ position.x - width / 2.f, position.y - height / 2.f, 0.f }, { position.x - width / 2.f, position.y + height / 2.f, 0.f });
	DrawLine({ position.x - width / 2.f, position.y - height / 2.f, 0.f }, { position.x + width / 2.f, position.y - height / 2.f, 0.f });
	DrawLine({ position.x + width / 2.f, position.y + height / 2.f, 0.f }, { position.x - width / 2.f, position.y + height / 2.f, 0.f });
	DrawLine({ position.x + width / 2.f, position.y + height / 2.f, 0.f }, { position.x + width / 2.f, position.y - height / 2.f, 0.f });
}

void DebugRenderer::DrawCircle(glm::vec3 position, float radius)
{
	s_cirP.push_back(position);
	s_cirR.push_back(radius);
}
