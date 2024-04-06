#pragma once
#include <memory>
#include "opengl/Shader.h"
#include "opengl/Texture.h"
#include <vector>

class MaterialComponent
{
public:
	MaterialComponent(std::string vertexShader, std::string fragmentShader);
	~MaterialComponent();

	void Bind(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
	void Unbind();
	void SetWorldMatrices(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
	Shader* GetShader();

private:
	std::string m_vertexShader;
	std::string m_fragmentShader;
};

