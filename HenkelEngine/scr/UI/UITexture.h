#pragma once
#include "UIArea.h"

class UITexture : public UIArea
{
public:
	UITexture();
	~UITexture();

	virtual void Update() override;
	virtual void Render(BatchRenderer* batchRenderer) override;

	void SetTexture(const std::string& texture);
	void SetColor(const glm::vec4& color);

private:

	std::string m_texture;
	glm::vec4 m_color;
};

