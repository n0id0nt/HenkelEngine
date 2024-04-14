#include "UIText.h"
#include "Engine.h"

UIText::UIText() : UIArea(), m_color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), m_font(), m_text(), m_size(0), m_textAlignment(TextHorizontalAlignment::Left), m_textWrapping(TextWrapping::Overflow)
{
    SetAnchorPoint(glm::vec2(0.f, 1.f));
}

UIText::~UIText()
{
}

void UIText::Update()
{
}

void UIText::Render(BatchRenderer* batchRenderer)
{
	glm::vec2 pos = GetLayoutPosition();
	Engine::GetInstance()->GetResourcePool()->RetrieveFont(m_font, m_size)->RenderFont(batchRenderer, m_text, pos.x, pos.y, GetWidth(), GetHeight(), m_textAlignment, m_textWrapping, m_color);
}

void UIText::SetText(const std::string& text)
{
	m_text = text;
}

void UIText::SetFont(const std::string& font, int size)
{
	if (m_font == font && m_size == size)
		return;
	Engine::GetInstance()->GetResourcePool()->ReleaseFont(m_font, m_size);
	m_size = size;
	m_font = font;
	Engine::GetInstance()->GetResourcePool()->CreateFont(m_font, m_size);
}

void UIText::SetColor(const glm::vec4& color)
{
	m_color = color;
}

void UIText::SetTextAlignment(TextHorizontalAlignment textAlignemt)
{
	m_textAlignment = textAlignemt;
}

void UIText::SetTextWrapping(TextWrapping textWrapping)
{
	m_textWrapping = textWrapping;
}
