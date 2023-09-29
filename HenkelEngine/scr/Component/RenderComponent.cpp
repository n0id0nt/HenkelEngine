#include "RenderComponent.h"
#include "..\Camera.h"
#include "../Entity/Entity.h"
#include "../Scene.h"
#include "../Window.h"
#include "../opengl/Texture.h"

RenderComponent::RenderComponent(Entity* entity)
    : Component(entity), m_Renderer("res/shaders/vertex.shader", "res/shaders/fragment.shader", "res/images/Zombie.png")
{
    GetEntity()->GetTransform()->SetScale({ m_Renderer.GetTexture()->GetWidth(), m_Renderer.GetTexture()->GetWidth(), 1.f });
}

void RenderComponent::Render()
{
    Camera* camera= GetEntity()->GetScene()->GetCamera();
    glm::mat4 model = GetEntity()->GetTransform()->GetWorldMatrix();
    glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 projection = camera->CalculateProjection(GetEntity()->GetScene()->GetWindow()->Width, GetEntity()->GetScene()->GetWindow()->Height);
    m_Renderer.Render(model, view, projection);
}
