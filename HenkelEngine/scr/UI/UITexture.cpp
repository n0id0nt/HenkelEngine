#include "UITexture.h"

UITexture::UITexture() : UIArea(), m_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), m_texture("blank")
{
}

UITexture::~UITexture()
{
}

void UITexture::Update()
{
}

void UITexture::Render(BatchRenderer* batchRenderer)
{
	batchRenderer->LoadTexture(m_texture);
	batchRenderer->AddTextureToBatch(m_texture, GetLayoutPosition(), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), GetAnchorPoint(), m_color, glm::vec2(GetWidth(), GetHeight()));
}

void UITexture::SetTexture(const std::string& texture)
{
	m_texture = texture;
}

void UITexture::SetColor(const glm::vec4& color)
{
	m_color = color;
}
