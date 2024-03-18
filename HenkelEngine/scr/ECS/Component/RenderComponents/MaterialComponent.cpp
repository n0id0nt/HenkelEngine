#include "MaterialComponent.h"
#include <glm\ext\matrix_transform.hpp>
#include "Engine.h"

MaterialComponent::MaterialComponent(std::string texture, std::string vertexShader, std::string fragmentShader)
    :  m_textures(), m_vertexShader(vertexShader), m_fragmentShader(fragmentShader)
{
    m_textures.push_back(texture);
    Engine::GetInstance()->GetResourcePool()->CreateShader(m_vertexShader, m_fragmentShader);
    Engine::GetInstance()->GetResourcePool()->CreateTexture(texture);
}

MaterialComponent::MaterialComponent(std::string vertexShader, std::string fragmentShader)
{
    m_textures.push_back("blank");
    Engine::GetInstance()->GetResourcePool()->CreateShader(m_vertexShader, m_fragmentShader);
}

MaterialComponent::~MaterialComponent()
{
    Engine::GetInstance()->GetResourcePool()->ReleaseShader(m_vertexShader, m_fragmentShader);
    for (int i = 0; i < m_textures.size(); i++)
    {
        Engine::GetInstance()->GetResourcePool()->ReleaseTexture(m_textures[i]);
    }
}

void MaterialComponent::Bind(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
    Engine::GetInstance()->GetResourcePool()->RetrieveShader(m_vertexShader, m_fragmentShader)->Bind();
    for (int i = 0; i < m_textures.size(); i++)
    {
        Engine::GetInstance()->GetResourcePool()->RetrieveTexture(m_textures[i])->Bind();
    }

    SetWorldMatrices(model, view, projection);

    //SetColor({ 1.f,1.f,1.f,1.f });
}

void MaterialComponent::Unbind()
{
    Engine::GetInstance()->GetResourcePool()->RetrieveShader(m_vertexShader, m_fragmentShader)->Unbind();
    for (int i = 0; i < m_textures.size(); i++)
    {
        Engine::GetInstance()->GetResourcePool()->RetrieveTexture(m_textures[i])->Unbind();
    }
}

void MaterialComponent::SetColor(glm::vec4 color)
{
    //m_Shader->SetUniform4f("spriteColor", color);
}

void MaterialComponent::SetWorldMatrices(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
    Shader* shader = Engine::GetInstance()->GetResourcePool()->RetrieveShader(m_vertexShader, m_fragmentShader);
    shader->SetUniformMat4f("U_Model", model);
    shader->SetUniformMat4f("U_ViewProjection", projection * view);
}
