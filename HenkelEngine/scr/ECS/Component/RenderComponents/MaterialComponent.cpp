#include "MaterialComponent.h"
#include <glm\ext\matrix_transform.hpp>
#include "Engine.h"

MaterialComponent::MaterialComponent(std::string texture, std::string vertexShader, std::string fragmentShader, Engine* engine)
    : m_engine(engine), m_texture(texture), m_vertexShader(vertexShader), m_fragmentShader(fragmentShader)
{
    m_engine->GetResourcePool()->CreateShader(m_vertexShader, m_fragmentShader);
    m_engine->GetResourcePool()->CreateTexture(m_texture);


    //m_Shader = std::make_unique<Shader>(vertexShader, fragmentShader);
    //m_Shader->Bind();
    //m_Shader->SetUniform1i("u_Texture", 0);
    //m_Texture = std::make_unique<Texture>(texture);
}

MaterialComponent::~MaterialComponent()
{
    m_engine->GetResourcePool()->ReleaseShader(m_vertexShader, m_fragmentShader);
    m_engine->GetResourcePool()->ReleaseTexture(m_texture);
}

void MaterialComponent::Bind(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
    m_engine->GetResourcePool()->RetriveShader(m_vertexShader, m_fragmentShader)->Bind();
    m_engine->GetResourcePool()->RetriveTexture(m_texture)->Bind();

    SetWorldMatrices(model, view, projection);

    SetColor({ 1.f,1.f,1.f,1.f });
}

void MaterialComponent::Unbind()
{
    m_engine->GetResourcePool()->RetriveShader(m_vertexShader, m_fragmentShader)->Unbind();
    m_engine->GetResourcePool()->RetriveTexture(m_texture)->Unbind();
}

void MaterialComponent::SetColor(glm::vec4 color)
{
    //m_Shader->SetUniform4f("spriteColor", color);
}

void MaterialComponent::SetWorldMatrices(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
    Shader* shader = m_engine->GetResourcePool()->RetriveShader(m_vertexShader, m_fragmentShader);
    shader->SetUniformMat4f("U_Model", model);
    shader->SetUniformMat4f("U_ViewProjection", projection * view);
}
