#include "BatchRenderer.h"
#include "Camera.h"
#include "Scene.h"
#include "Window.h"

BatchRenderer::BatchRenderer(unsigned int quads) : m_Quads(quads), m_batchValid(false)
{
    // ensure renderer will actually draw something
    ASSERT(quads > 0);
    InitRenderData();
}

BatchRenderer::~BatchRenderer()
{
    GLCall(glDeleteVertexArrays(1, &m_VAO));
    GLCall(glDeleteBuffers(1, &m_VBO));
    GLCall(glDeleteBuffers(1, &m_IBO));
}

void BatchRenderer::Render()
{
    // Bind Material

    GLCall(glBindVertexArray(m_VAO));

    // loop to ensure no more than the max batch size is rendered
    int batchesToDraw = (int)glm::ceil((float)m_Vertices.size() / (m_Quads * QUAD_SIZE));
    int indexOffset = 0;
    for (int i = 0; i < batchesToDraw; i++)
    {

        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * glm::min(static_cast<int>(m_Quads * QUAD_SIZE), static_cast<int>(m_Vertices.size() - indexOffset)), &m_Vertices.at(indexOffset)));

        GLCall(glDrawElements(GL_TRIANGLES, m_Indexes.size(), GL_UNSIGNED_INT, 0));

        indexOffset += m_Quads * QUAD_SIZE;
    }
    GLCall(glBindVertexArray(0));
}

void BatchRenderer::AddQuadToBatch(const glm::vec2& pos, const glm::vec2& uvPos, const glm::vec2& uvSize, const glm::vec2& anchorPoint, const glm::vec4& color, const glm::vec2& size)
{
    ASSERT(IsBatching());
    std::vector<Vertex> vertices;
    Mesh::setQuadData(vertices, pos, { uvPos, uvSize }, false, anchorPoint, color, size);
    m_Vertices.insert(m_Vertices.end(), vertices.begin(), vertices.end());
}

void BatchRenderer::SetQuadUVs(const glm::vec4& rect, const bool& flipped)
{
    ASSERT(!IsBatching()); // can't be batching because batch is cleared each draw call 
    Mesh::setQuadData(m_Vertices, { 0.f, 0.f }, rect, flipped);
}

bool BatchRenderer::BatchValid()
{
    return m_batchValid;
}

void BatchRenderer::InvalidateBatch()
{
    m_batchValid = false;
}

void BatchRenderer::ValidateBatch()
{
    m_batchValid = m_Vertices.size();
}

void BatchRenderer::ClearBatch()
{
    m_Vertices.clear();
}

void BatchRenderer::InitRenderData()
{
    Mesh::setQuadData(m_Indexes, m_Quads);

    // init the VAO and VBO
    GLCall(glGenVertexArrays(1, &m_VAO));
    GLCall(glBindVertexArray(m_VAO));

    GLCall(glGenBuffers(1, &m_VBO));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));

    // only bother with batching when more than one quad needs to be drawn
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * QUAD_SIZE * m_Quads, nullptr, GL_DYNAMIC_DRAW));
    //GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_Vertices.size(), m_Vertices.data(), GL_STATIC_DRAW));

    GLCall(glGenBuffers(1, &m_IBO));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_Indexes.size(), m_Indexes.data(), GL_STATIC_DRAW));

    // Position attribute
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0));

    // Color attribute
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, Vertex::color))));

    // TexCoord attribute
    GLCall(glEnableVertexAttribArray(2));
    GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, Vertex::texCoord))));    
    
    // TexIndex attribute
    GLCall(glEnableVertexAttribArray(3));
    GLCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), (GLvoid*)(offsetof(Vertex, Vertex::texIndex))));

    GLCall(glBindVertexArray(0)); // Unbind VAO
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); // Unbind VBO
}

bool BatchRenderer::IsBatching()
{
    return m_Quads > 1;
}
