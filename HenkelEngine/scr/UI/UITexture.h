#pragma once
#include "UIArea.h"

class UITexture : public UIArea
{
public:
	UITexture();
	~UITexture();

	virtual void Update() override;
	virtual void Render(BatchRenderer* batchRenderer) override;

	void SetColor(glm::vec4 color);

private:

	glm::vec4 m_color;
};

