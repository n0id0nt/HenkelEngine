#include "DebugRenderer.h"
#include <GL\glew.h> 
#include <glm\ext\scalar_constants.hpp>
#include <glm\trigonometric.hpp>

#define DEBUG_BUFFER_SIZE 2000

GLuint DebugRenderer::s_VAO;
GLuint DebugRenderer::s_VBO;

std::vector<DebugVertex> DebugRenderer::s_verticesWS;
std::vector<DebugVertex> DebugRenderer::s_verticesSS;

Shader* DebugRenderer::s_shaderWS;
Shader* DebugRenderer::s_shaderSS;

void DebugRenderer::Render(glm::mat4 viewProjection)
{
	s_shaderWS->Bind();
	s_shaderWS->SetUniformMat4f("U_ViewProjection", viewProjection);
	RenderVertices(s_verticesWS, s_VAO, s_VBO);
	s_verticesWS.clear();
	s_shaderWS->Unbind();

	s_shaderSS->Bind();
	RenderVertices(s_verticesSS, s_VAO, s_VBO);
	s_verticesSS.clear();
}

void DebugRenderer::RenderVertices(std::vector<DebugVertex>& vertices, GLuint& VAO, GLuint& VBO)
{
	GLCall(glBindVertexArray(s_VAO));

	// loop to ensure no more than the max batch size is rendered
	int batchesToDraw = (int)glm::ceil((float)vertices.size() / DEBUG_BUFFER_SIZE);
	for (int i = 0; i < batchesToDraw; i++)
	{

		GLCall(glBindBuffer(GL_ARRAY_BUFFER, s_VBO));
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(DebugVertex) * glm::min((size_t)DEBUG_BUFFER_SIZE, vertices.size()), vertices.data()));

		GLCall(glDrawArrays(GL_LINES, 0, vertices.size()));

		if (i != batchesToDraw - 1)
		{
			vertices.erase(vertices.begin(), vertices.begin() + DEBUG_BUFFER_SIZE);
		}

	}
	GLCall(glBindVertexArray(0));
}

void DebugRenderer::InitVAOandVBO(GLuint& VAO, GLuint& VBO)
{
	// init the VAO and VBO
	GLCall(glGenVertexArrays(1, &VAO));
	GLCall(glBindVertexArray(VAO));

	GLCall(glGenBuffers(1, &VBO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));

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

void DebugRenderer::InitDebugRenderer()
{
	// Create Shaders
	s_shaderWS = new Shader("res/shaders/debug_ws.vert", "res/shaders/debug_ws.frag");
	s_shaderSS = new Shader("res/shaders/debug_ss.vert", "res/shaders/debug_ss.frag");

	InitVAOandVBO(s_VAO, s_VBO);
}

void DebugRenderer::ExitDebugRenderer()
{
	GLCall(glDeleteVertexArrays(1, &s_VAO));
	GLCall(glDeleteBuffers(1, &s_VBO));
	delete s_shaderWS;
}

void DebugRenderer::DrawLine(glm::vec3 point1, glm::vec3 point2, glm::vec3 color)
{
	s_verticesWS.push_back(DebugVertex{ point1, color });
	s_verticesWS.push_back(DebugVertex{ point2, color });
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

void DebugRenderer::DrawScreenLine(glm::vec3 point1, glm::vec3 point2, glm::vec3 color)
{
	s_verticesSS.push_back(DebugVertex{ point1, color });
	s_verticesSS.push_back(DebugVertex{ point2, color });
}

void DebugRenderer::DrawScreenRectangle(glm::vec3 position, float width, float height, glm::vec3 color)
{
	DrawScreenLine({ position.x - width / 2.f, position.y - height / 2.f, 0.f }, { position.x - width / 2.f, position.y + height / 2.f, 0.f }, color);
	DrawScreenLine({ position.x - width / 2.f, position.y - height / 2.f, 0.f }, { position.x + width / 2.f, position.y - height / 2.f, 0.f }, color);
	DrawScreenLine({ position.x + width / 2.f, position.y + height / 2.f, 0.f }, { position.x - width / 2.f, position.y + height / 2.f, 0.f }, color);
	DrawScreenLine({ position.x + width / 2.f, position.y + height / 2.f, 0.f }, { position.x + width / 2.f, position.y - height / 2.f, 0.f }, color);
}

void DebugRenderer::DrawScreenCircle(glm::vec3 position, float radius, glm::vec3 color)
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

		DrawScreenLine(glm::vec3{ x1, y1, 0.f } + position, glm::vec3{ x2, y2, 0.f } + position, color);
	}
}
