#include "MaterialComponent.h"
#include <glm\ext\matrix_transform.hpp>

MaterialComponent::MaterialComponent(std::string texture, std::string vertexShader, std::string fragmentShader)
{
    m_Shader = std::make_unique<Shader>(vertexShader, fragmentShader);
    m_Shader->Bind();
    m_Shader->SetUniform1i("u_Texture", 0);
    m_Texture = std::make_unique<Texture>(texture);
}

void MaterialComponent::Bind(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
    m_Shader->Bind();
    m_Texture->Bind();

    SetWorldMatrices(model, view, projection);

    SetColor({ 1.f,1.f,1.f,1.f });
}

void MaterialComponent::Unbind()
{
    m_Shader->Unbind();
    m_Texture->Unbind();
}

void MaterialComponent::SetColor(glm::vec4 color)
{
    //m_Shader->SetUniform4f("spriteColor", color);
}

void MaterialComponent::SetWorldMatrices(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
    m_Shader->SetUniformMat4f("U_Model", model);
    m_Shader->SetUniformMat4f("U_ViewProjection", projection * view);
}
