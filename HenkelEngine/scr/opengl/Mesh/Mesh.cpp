#include "Mesh.h"
#include "../../opengl/Renderer.h"

void Mesh::setCubeData(std::vector<Vertex>& vertices)
{
    std::vector<Vertex> _vertices =
    {
        // Position             Color               Texture
        {{-0.5f, -0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 0.0f}},
                              
        {{-0.5f, -0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 0.0f}},
  
        {{-0.5f,  0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 0.0f}},
      
        {{ 0.5f,  0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 0.0f}},

        {{-0.5f, -0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 0.0f}},
        {{-0.5f, -0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 1.0f}},

        {{-0.5f,  0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 1.0f}} 
    };

    vertices.clear();
    vertices = _vertices;
}

void Mesh::setQuadData(std::vector<Vertex>& vertices, std::vector<GLuint>& indexes, unsigned int quadsCount)
{
    setQuadData(vertices, { 0.f, 0.f }, { 0.f, 0.f, 1.f, 1.f });
    setQuadData(indexes, quadsCount);
}

void Mesh::setQuadData(std::vector<GLuint>& indexes, unsigned int quadsCount)
{
    std::vector<GLuint> _indexes =
    {
        2, 0, 1, 1, 3, 2,
    };

    indexes.clear();
    int offset = 0;
    for (int i = 0; i < quadsCount; i++)
    {
        for (auto& index : _indexes)
        {
            indexes.push_back(index + offset);
        }
        offset += QUAD_SIZE;
    }
}

void Mesh::setQuadData(std::vector<Vertex>& vertices, const glm::vec2& pos, const glm::vec4& sourceRect)
{
    std::vector<Vertex> _vertices =
    {
        // Position             Color               Texture
        {{pos.x - 0.5f, pos.y - 0.5f, 0.f},   {1.f,1.f,1.f,1.f},  {sourceRect.x, sourceRect.y}},
        {{pos.x + 0.5f, pos.y - 0.5f, 0.f},   {1.f,1.f,1.f,1.f},  {sourceRect.z, sourceRect.y}},
        {{pos.x - 0.5f, pos.y + 0.5f, 0.f},   {1.f,1.f,1.f,1.f},  {sourceRect.x, sourceRect.w}},
        {{pos.x + 0.5f, pos.y + 0.5f, 0.f},   {1.f,1.f,1.f,1.f},  {sourceRect.z, sourceRect.w}},
    };

    vertices.clear();
    vertices = _vertices;
}
