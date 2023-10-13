#pragma once
#include <memory>
#include "Shader.h"
#include "Texture.h"

class Material
{
public:
	Material(std::string texture, std::string vertexShader, std::string fragmentShader);
	
	void Bind(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
	void SetColor(glm::vec4 color);
	void SetWorldMatrices(glm::mat4 model, glm::mat4 view, glm::mat4 projection);

private:
	std::unique_ptr<Shader> m_Shader;
	std::unique_ptr<Texture> m_Texture;
};

