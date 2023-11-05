#include "MaterialComponent.h"
#include <glm\ext\matrix_transform.hpp>
#include "Engine.h"

MaterialComponent::MaterialComponent(std::string texture, std::string vertexShader, std::string fragmentShader)
    :  m_texture(texture), m_vertexShader(vertexShader), m_fragmentShader(fragmentShader)
{
    Engine::GetInstance()->GetResourcePool()->CreateShader(m_vertexShader, m_fragmentShader);
    Engine::GetInstance()->GetResourcePool()->CreateTexture(m_texture);


    //m_Shader = std::make_unique<Shader>(vertexShader, fragmentShader);
    //m_Shader->Bind();
    //m_Shader->SetUniform1i("u_Texture", 0);
    //m_Texture = std::make_unique<Texture>(texture);
}

MaterialComponent::~MaterialComponent()
{
    Engine::GetInstance()->GetResourcePool()->ReleaseShader(m_vertexShader, m_fragmentShader);
    Engine::GetInstance()->GetResourcePool()->ReleaseTexture(m_texture);
}

void MaterialComponent::Bind(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
    Engine::GetInstance()->GetResourcePool()->RetriveShader(m_vertexShader, m_fragmentShader)->Bind();
    Engine::GetInstance()->GetResourcePool()->RetriveTexture(m_texture)->Bind();

    SetWorldMatrices(model, view, projection);

    SetColor({ 1.f,1.f,1.f,1.f });
}

void MaterialComponent::Unbind()
{
    Engine::GetInstance()->GetResourcePool()->RetriveShader(m_vertexShader, m_fragmentShader)->Unbind();
    Engine::GetInstance()->GetResourcePool()->RetriveTexture(m_texture)->Unbind();
}

void MaterialComponent::SetColor(glm::vec4 color)
{
    //m_Shader->SetUniform4f("spriteColor", color);
}

void MaterialComponent::SetWorldMatrices(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
    Shader* shader = Engine::GetInstance()->GetResourcePool()->RetriveShader(m_vertexShader, m_fragmentShader);
    shader->SetUniformMat4f("U_Model", model);
    shader->SetUniformMat4f("U_ViewProjection", projection * view);
}
