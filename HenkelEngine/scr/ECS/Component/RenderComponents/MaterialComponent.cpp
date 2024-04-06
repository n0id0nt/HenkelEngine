#include "MaterialComponent.h"
#include <glm\ext\matrix_transform.hpp>
#include "Engine.h"

MaterialComponent::MaterialComponent(std::string vertexShader, std::string fragmentShader)
    : m_vertexShader(vertexShader), m_fragmentShader(fragmentShader)
{
    Engine::GetInstance()->GetResourcePool()->CreateShader(m_vertexShader, m_fragmentShader);
}

MaterialComponent::~MaterialComponent()
{
    Engine::GetInstance()->GetResourcePool()->ReleaseShader(m_vertexShader, m_fragmentShader);
}

void MaterialComponent::Bind(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
    Engine::GetInstance()->GetResourcePool()->RetrieveShader(m_vertexShader, m_fragmentShader)->Bind();
    SetWorldMatrices(model, view, projection);
}

void MaterialComponent::Unbind()
{
    Engine::GetInstance()->GetResourcePool()->RetrieveShader(m_vertexShader, m_fragmentShader)->Unbind();
}

void MaterialComponent::SetWorldMatrices(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
    Shader* shader = Engine::GetInstance()->GetResourcePool()->RetrieveShader(m_vertexShader, m_fragmentShader);
    shader->SetUniformMat4f("U_Model", model);
    shader->SetUniformMat4f("U_ViewProjection", projection * view);
}

Shader* MaterialComponent::GetShader()
{
    return Engine::GetInstance()->GetResourcePool()->RetrieveShader(m_vertexShader, m_fragmentShader);
}
