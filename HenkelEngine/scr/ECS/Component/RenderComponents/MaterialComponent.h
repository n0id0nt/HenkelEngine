#pragma once
#include <memory>
#include "opengl/Shader.h"
#include "opengl/Texture.h"

class Engine;

class MaterialComponent
{
public:
	MaterialComponent(std::string texture, std::string vertexShader, std::string fragmentShader, Engine* engine);
	~MaterialComponent();

	void Bind(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
	void Unbind();
	void SetColor(glm::vec4 color);
	void SetWorldMatrices(glm::mat4 model, glm::mat4 view, glm::mat4 projection);

private:
	Engine* m_engine = nullptr;

	std::string m_vertexShader;
	std::string m_fragmentShader;
	std::string m_texture;
};

