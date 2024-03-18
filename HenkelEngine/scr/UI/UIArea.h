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
	glm::vec2 GetLayoutPosition();

	void SetAnchorPoint(glm::vec2 anchorPoint);
	glm::vec2 GetAnchorPoint();

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

	UIArea* GetParent();

	glm::mat4 GetMatrix();
	glm::mat4 GetLayoutMatrix();

private:
	float m_x, m_y;
	float m_width, m_height;
	glm::vec2 m_anchorPoint;

	std::vector<std::unique_ptr<UIArea>> m_children;//TODO change the ownership of children from the area to the component layout
	UIArea* m_parent = nullptr;
};

