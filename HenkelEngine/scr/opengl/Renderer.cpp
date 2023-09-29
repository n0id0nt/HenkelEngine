#include "Renderer.h"
#include "../Entity/Entity.h"
#include "../Camera.h"
#include "../Scene.h"
#include "../Window.h"
#include "../opengl/Mesh/Mesh.h"


Renderer::Renderer(std::string vertexShader, std::string fragmentShader, std::string texture)
{
    m_Shader = std::make_unique<Shader>(vertexShader, fragmentShader);
    m_Shader->Bind();
    m_Shader->SetUniform1i("image", 0);
    m_Texture = std::make_unique<Texture>(texture);

    initRenderData();
}

void Renderer::Render(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
    // Bind Textures using texture units
    m_Shader->Bind();
    m_Texture->Bind();

    // Set world matrices 
    m_Shader->SetUniformMat4f("model", model);
    m_Shader->SetUniformMat4f("view", view);
    m_Shader->SetUniformMat4f("projection", projection);

    GLCall(glBindVertexArray(m_VAO));

    m_Shader->SetUniform4f("spriteColor", {1.f,1.f,1.f,1.f});
    GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

    GLCall(glBindVertexArray(0));
}

Texture* Renderer::GetTexture()
{
    return m_Texture.get();
}

void Renderer::initRenderData()
{
    GLuint VBO;    
    Mesh::setQuadData(m_Vertices);

    // init the VAO and VBO
    GLCall(glGenVertexArrays(1, &m_VAO));
    GLCall(glBindVertexArray(m_VAO));

    GLCall(glGenBuffers(1, &VBO));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_Vertices.size(), &m_Vertices[0], GL_STATIC_DRAW));

    // Position attribute
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0));

    // TexCoord attribute
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, Vertex::texCoord))));

    GLCall(glBindVertexArray(0)); // Unbind VAO
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); // Unbind VBO
}
