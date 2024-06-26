#include "UIQuad.h"

UIQuad::UIQuad() : UIArea(), m_color(glm::vec4(1.0f,1.0f,1.0f,1.0f))
{
}

UIQuad::~UIQuad()
{
}

void UIQuad::Update()
{
}

void UIQuad::Render(BatchRenderer* batchRenderer)
{
	batchRenderer->LoadTexture("blank");
	batchRenderer->AddTextureToBatch("blank", GetLayoutPosition(), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), GetAnchorPoint(), m_color, glm::vec2(GetWidth(), GetHeight()));
}

void UIQuad::SetColor(const glm::vec4& color)
{
	m_color = color;
}
