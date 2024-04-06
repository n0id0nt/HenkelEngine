#include "RenderComponent.h"
#include "Camera.h"
#include "Scene.h"
#include "Window.h"

RenderComponent::RenderComponent(unsigned int quads) : m_batchRenderer(quads)
{
}

RenderComponent::~RenderComponent()
{
}

void RenderComponent::Render()
{
    m_batchRenderer.Render();
}

void RenderComponent::LoadTexture(const std::string& texture)
{
    m_batchRenderer.LoadTexture(texture);
}

void RenderComponent::UnloadTexture(const std::string& texture)
{
    m_batchRenderer.UnloadTexture(texture);
}

void RenderComponent::BindTextures()
{
    m_batchRenderer.BindTextures();
}

void RenderComponent::UnbindTextures()
{
    m_batchRenderer.UnbindTextures();
}

void RenderComponent::AddTextureToBatch(const std::string& texture, const glm::vec2& pos, const glm::vec2& uvPos, const glm::vec2& uvSize)
{
    m_batchRenderer.AddTextureToBatch(texture, pos, uvPos, uvSize);
}

void RenderComponent::AddQuadToBatch(const glm::vec2& pos, const glm::vec2& uvPos, const glm::vec2& uvSize)
{
    m_batchRenderer.AddQuadToBatch(pos, uvPos, uvSize);
}

void RenderComponent::SetQuadUVs(const glm::vec4& rect, const bool& flipped)
{
    m_batchRenderer.SetQuadUVs(rect, flipped);
}

bool RenderComponent::BatchValid()
{
    return m_batchRenderer.BatchValid();
}

void RenderComponent::InvalidateBatch()
{
    m_batchRenderer.InvalidateBatch();
}

void RenderComponent::ValidateBatch()
{
    m_batchRenderer.ValidateBatch();
}

void RenderComponent::ClearBatch()
{
    m_batchRenderer.ClearBatch();
}

BatchRenderer* RenderComponent::GetBatchRenderer()
{
    return &m_batchRenderer;
}
