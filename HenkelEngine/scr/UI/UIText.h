#pragma once
#include "UIArea.h"

class UIText : public UIArea
{
public:
	UIText();
	~UIText();

	virtual void Update() override;
	virtual void Render(BatchRenderer* batchRenderer) override;

	void SetFont(const std::string& font, int size);
	void SetText(const std::string& text);
	void SetColor(const glm::vec4& color);

private:

	std::string m_font;
	int m_size;
	std::string m_text;
	glm::vec4 m_color;
};

