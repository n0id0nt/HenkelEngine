#include "UIArea.h"
#include "opengl\DebugRenderer.h"
UIArea::UIArea() : m_children(), m_x(0.0f), m_y(0.0f), m_width(0.0f), m_height(0.0f)
{
}

void UIArea::Update()
{
	DebugRenderer::DrawScreenRectangle(glm::vec3(m_x, m_y, 0.0f), m_width, m_height);
}

void UIArea::Render()
{
}

void UIArea::AddChild(std::unique_ptr<UIArea> area)
{
	m_children.push_back(std::move(area));
}

std::vector<UIArea*> UIArea::GetChildren()
{
	std::vector<UIArea*> children;
	for (auto& child : m_children)
	{
		children.push_back(child.get());
	}
	return children;
}

void UIArea::SetPosition(glm::vec2 position)
{
	m_x = position.x;
	m_y = position.y;
}

glm::vec2 UIArea::GetPosition()
{
	return glm::vec2(m_x, m_y);
}

void UIArea::SetDimensions(glm::vec2 dimensions)
{
	m_width = dimensions.x;
	m_height = dimensions.y;
}

glm::vec2 UIArea::GetDimensions()
{
	return glm::vec2(m_width, m_height);
}

void UIArea::SetX(float x)
{
	m_x = x;
}

float UIArea::GetX()
{
	return m_x;
}

void UIArea::SetY(float y)
{
	m_y = y;
}

float UIArea::GetY()
{
	return m_y;
}

void UIArea::SetWidth(float width)
{
	m_width = width;
}

float UIArea::GetWidth()
{
	return m_width;
}

void UIArea::SetHeight(float height)
{
	m_height = height;
}

float UIArea::GetHeight()
{
	return m_height;
}
