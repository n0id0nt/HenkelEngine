#pragma once

#include <vector>
#include "glm\glm.hpp"
#include <memory>
#include "opengl\BatchRenderer.h"

class UIArea
{
public:
	UIArea();
	~UIArea();

	virtual void Update();
	virtual void Render(BatchRenderer* batchRenderer);

	void AddChild(std::unique_ptr<UIArea> area);

	std::vector<UIArea*> GetChildren();

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

	std::vector<std::unique_ptr<UIArea>> m_children;
};

