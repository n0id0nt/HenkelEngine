#pragma once

#include <vector>
#include "glm\glm.hpp"

class UIArea
{
public:
	UIArea();

	void Update();
	void Render();

	void AddChild(UIArea area);

	std::vector<UIArea> GetChildren();

	void SetPosition(glm::vec2 position);
	glm::vec2 GetPosition();

	void SetDimensions(glm::vec2 dimensions);
	glm::vec2 GetDimensions();

	void SetX(float x);
	float GetX();

	void SetY(float y);
	float GetY();

	void SetWidth(float width);
	float GetWidth();

	void SetHeight(float height);
	float GetHeight();

private:
	float m_x, m_y;
	float m_width, m_height;

	std::vector<UIArea> m_children;


};

