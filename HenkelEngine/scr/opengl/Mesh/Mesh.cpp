#include "Mesh.h"

void Mesh::setCubeData(std::vector<Vertex>& vertices)
{
    std::vector<Vertex> _vertices =
    {
        // Position             Color               Texture UVs   Texture Index
        {{-0.5f, -0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 0.0f}, 0},
        {{ 0.5f, -0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 0.0f}, 0},
        {{ 0.5f,  0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 1.0f}, 0},
        {{ 0.5f,  0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 1.0f}, 0},
        {{-0.5f,  0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 1.0f}, 0},
        {{-0.5f, -0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 0.0f}, 0},

        {{-0.5f, -0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 0.0f}, 0},
        {{ 0.5f, -0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 0.0f}, 0},
        {{ 0.5f,  0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 1.0f}, 0},
        {{ 0.5f,  0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 1.0f}, 0},
        {{-0.5f,  0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 1.0f}, 0},
        {{-0.5f, -0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 0.0f}, 0},

        {{-0.5f,  0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 0.0f}, 0},
        {{-0.5f,  0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 1.0f}, 0},
        {{-0.5f, -0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 1.0f}, 0},
        {{-0.5f, -0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 1.0f}, 0},
        {{-0.5f, -0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 0.0f}, 0},
        {{-0.5f,  0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 0.0f}, 0},

        {{ 0.5f,  0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 0.0f}, 0},
        {{ 0.5f,  0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 1.0f}, 0},
        {{ 0.5f, -0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 1.0f}, 0},
        {{ 0.5f, -0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 1.0f}, 0},
        {{ 0.5f, -0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 0.0f}, 0},
        {{ 0.5f,  0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 0.0f}, 0},

        {{-0.5f, -0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 1.0f}, 0},
        {{ 0.5f, -0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 1.0f}, 0},
        {{ 0.5f, -0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 0.0f}, 0},
        {{ 0.5f, -0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 0.0f}, 0},
        {{-0.5f, -0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 0.0f}, 0},
        {{-0.5f, -0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 1.0f}, 0},

        {{-0.5f,  0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 1.0f}, 0},
        {{ 0.5f,  0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 1.0f}, 0},
        {{ 0.5f,  0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 0.0f}, 0},
        {{ 0.5f,  0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {1.0f, 0.0f}, 0},
        {{-0.5f,  0.5f,  0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 0.0f}, 0},
        {{-0.5f,  0.5f, -0.5f}, {1.f,1.f,1.f,1.f},  {0.0f, 1.0f}, 0} 
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
    static const std::vector<GLuint> _indexes =
    {
        2, 0, 1, 1, 3, 2,
    };

    indexes.clear();
    int offset = 0;
    for (unsigned int i = 0; i < quadsCount; i++)
    {
        for (auto& index : _indexes)
        {
            indexes.push_back(index + offset);
        }
        offset += QUAD_SIZE;
    }
}

void Mesh::setQuadData(std::vector<Vertex>& vertices, const glm::vec2& pos, const glm::vec4& sourceRect, const bool& flipped, const glm::vec2& anchorPoint, const glm::vec4& color, const glm::vec2& size, int textureIndex)
{
    int flippedMultiplier = flipped ? -1 : 1;
    std::vector<Vertex> _vertices =
    {
        // Position                                                                                                         Color   Texture UVs                   Texture Index
        {{pos.x + (-size.x * anchorPoint.x)         * flippedMultiplier, pos.y + (-size.y * anchorPoint.y),         0.f},   color,  {sourceRect.x, sourceRect.y}, textureIndex},
        {{pos.x + ((1.0f - anchorPoint.x) * size.x) * flippedMultiplier, pos.y + (-size.y * anchorPoint.y),         0.f},   color,  {sourceRect.z, sourceRect.y}, textureIndex},
        {{pos.x + (-size.x * anchorPoint.x)         * flippedMultiplier, pos.y + ((1.0f - anchorPoint.y) * size.y), 0.f},   color,  {sourceRect.x, sourceRect.w}, textureIndex},
        {{pos.x + ((1.0f - anchorPoint.x) * size.x) * flippedMultiplier, pos.y + ((1.0f - anchorPoint.y) * size.y), 0.f},   color,  {sourceRect.z, sourceRect.w}, textureIndex},
    };

    vertices.clear();
    vertices = _vertices;
}
