#include "Material.h"
#include <glm\ext\matrix_transform.hpp>

Material::Material(std::string texture, std::string vertexShader, std::string fragmentShader)
{
    m_Shader = std::make_unique<Shader>(vertexShader, fragmentShader);
    m_Shader->Bind();
    m_Shader->SetUniform1i("u_Texture", 0);
    m_Texture = std::make_unique<Texture>(texture);
}

void Material::Bind(glm::mat4 model, glm::mat4 view, glm::mat4 projection, bool stretchToImageSize)
{
    m_Shader->Bind();
    m_Texture->Bind();

    if (stretchToImageSize)
        model = glm::scale(model, {m_Texture->GetWidth(), m_Texture->GetHeight(), 1.f});

    SetWorldMatrices(model, view, projection);

    SetColor({ 1.f,1.f,1.f,1.f });
}

void Material::SetColor(glm::vec4 color)
{
    //m_Shader->SetUniform4f("spriteColor", color);
}

void Material::SetWorldMatrices(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
    m_Shader->SetUniformMat4f("U_Model", model);
    m_Shader->SetUniformMat4f("U_ViewProjection", projection * view);
}
