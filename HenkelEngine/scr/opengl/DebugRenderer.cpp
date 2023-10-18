#include "DebugRenderer.h"
#include <GL\glew.h> 
#include <glm\ext\scalar_constants.hpp>
#include <glm\trigonometric.hpp>

#define DEBUG_BUFFER_SIZE 1000 * 2

GLuint DebugRenderer::s_VAO;
GLuint DebugRenderer::s_VBO;
std::vector<DebugVertex> DebugRenderer::s_Vertices;

Shader* DebugRenderer::s_shader;

void DebugRenderer::Render(glm::mat4 viewProjection)
{
	s_shader->Bind();
	s_shader->SetUniformMat4f("U_ViewProjection", viewProjection);

	GLCall(glBindVertexArray(s_VAO));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, s_VBO));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(DebugVertex) * s_Vertices.size(), s_Vertices.data()));

	GLCall(glDrawArrays(GL_LINES, 0, s_Vertices.size()));

	GLCall(glBindVertexArray(0));

	s_Vertices.clear();
}

void DebugRenderer::InitDebugRenderer()
{
	// Create Shaders
	s_shader = new Shader("res/shaders/debug.vert", "res/shaders/debug.frag");
	s_shader->Bind();

	// init the VAO and VBO
	GLCall(glGenVertexArrays(1, &s_VAO));
	GLCall(glBindVertexArray(s_VAO));

	GLCall(glGenBuffers(1, &s_VBO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, s_VBO));

	// only bother with batching when more than one quad needs to be drawn
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(DebugVertex) * DEBUG_BUFFER_SIZE, nullptr, GL_DYNAMIC_DRAW));
	//GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(DebugVertex) * s_Vertices.size(), s_Vertices.data(), GL_STATIC_DRAW));

	// Position attribute
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (GLvoid*)0));

	// Color attribute
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (GLvoid*)(offsetof(DebugVertex, DebugVertex::color))));

	GLCall(glBindVertexArray(0)); // Unbind VAO
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); // Unbind VBO
}

void DebugRenderer::ExitDebugRenderer()
{
	GLCall(glDeleteVertexArrays(1, &s_VAO));
	GLCall(glDeleteBuffers(1, &s_VBO));
	delete s_shader;
}

void DebugRenderer::DrawLine(glm::vec3 point1, glm::vec3 point2, glm::vec3 color)
{
	s_Vertices.push_back(DebugVertex{ point1, color });
	s_Vertices.push_back(DebugVertex{ point2, color });
}

void DebugRenderer::DrawRectangle(glm::vec3 position, float width, float height, glm::vec3 color)
{
	DrawLine({ position.x - width / 2.f, position.y - height / 2.f, 0.f }, { position.x - width / 2.f, position.y + height / 2.f, 0.f }, color);
	DrawLine({ position.x - width / 2.f, position.y - height / 2.f, 0.f }, { position.x + width / 2.f, position.y - height / 2.f, 0.f }, color);
	DrawLine({ position.x + width / 2.f, position.y + height / 2.f, 0.f }, { position.x - width / 2.f, position.y + height / 2.f, 0.f }, color);
	DrawLine({ position.x + width / 2.f, position.y + height / 2.f, 0.f }, { position.x + width / 2.f, position.y - height / 2.f, 0.f }, color);
}

void DebugRenderer::DrawCircle(glm::vec3 position, float radius, glm::vec3 color)
{
	const int definition = 30;
	for (int i = 0; i < definition; i++)
	{
		float angle1 = i * (2.0f * glm::pi<float>()) / definition;
		float x1 = radius * glm::sin(angle1);
		float y1 = radius * glm::cos(angle1);
		
		float angle2 = (i + 1) * (2.0f * glm::pi<float>()) / definition;
		float x2 = radius * glm::sin(angle2);
		float y2 = radius * glm::cos(angle2);

		DrawLine(glm::vec3{ x1 , y1, 0.f } + position, glm::vec3{ x2, y2, 0.f } + position, color);
	}
}
