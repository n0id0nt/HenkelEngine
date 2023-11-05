#pragma once
#include <memory>
#include "opengl/Shader.h"
#include "opengl/Texture.h"

class MaterialComponent
{
public:
	MaterialComponent(std::string texture, std::string vertexShader, std::string fragmentShader);
	~MaterialComponent();

	void Bind(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
	void Unbind();
	void SetColor(glm::vec4 color);
	void SetWorldMatrices(glm::mat4 model, glm::mat4 view, glm::mat4 projection);

private:
	std::string m_vertexShader;
	std::string m_fragmentShader;
	std::string m_texture;
};

