#include "UIArea.h"
#include "opengl\DebugRenderer.h"
#include <glm\gtx\transform.hpp>
#include <glm\gtx\matrix_decompose.hpp>

UIArea::UIArea() : m_children(), m_x(0.0f), m_y(0.0f), m_width(0.0f), m_height(0.0f), m_anchorPoint(glm::vec2(0.0f, 0.0f))
{
}

UIArea::~UIArea()
{
}

void UIArea::Update()
{
}

void UIArea::Render(BatchRenderer* batchRenderer)
{
}

void UIArea::AddChild(std::unique_ptr<UIArea> area)
{
	//TODO remove the area from the parents children
	area->m_parent = this;
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

glm::vec2 UIArea::GetLayoutPosition()
{
	glm::vec3 scale;
	glm::quat rotation{};
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(GetLayoutMatrix(), scale, rotation, translation, skew, perspective);
	return translation;
}

void UIArea::SetAnchorPoint(glm::vec2 anchorPoint)
{
	m_anchorPoint = anchorPoint;
}

glm::vec2 UIArea::GetAnchorPoint()
{
	return m_anchorPoint;
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

UIArea* UIArea::GetParent()
{
	return m_parent;
}

glm::mat4 UIArea::GetMatrix()
{
	glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3{ GetPosition(), 0.0f });

	return translation;
}

glm::mat4 UIArea::GetLayoutMatrix()
{
	if (GetParent())
		return GetParent()->GetLayoutMatrix() * GetMatrix();
	else
		return GetMatrix();
}

void UIArea::LUABind(sol::state& lua)
{
	lua.new_usertype<UIArea>("uiArea",
		"setX", &UIArea::SetX,
		"getX", &UIArea::GetX
	);
}
