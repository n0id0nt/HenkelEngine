#include "RenderComponent.h"
#include "..\Camera.h"
#include "../Entity/Entity.h"
#include "../Scene.h"
#include "../Window.h"
#include "../opengl/Texture.h"

RenderComponent::RenderComponent(Entity* entity, unsigned int QuadCound, std::string texture)
    : Component(entity), m_Renderer(new Material(texture, "res/shaders/sprite.vert", "res/shaders/sprite.frag"), QuadCound)
{
}

void RenderComponent::Render()
{
    Camera* camera= GetEntity()->GetScene()->GetCamera();
    glm::mat4 model = GetEntity()->GetTransform()->GetWorldMatrix();
    glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 projection = camera->CalculateProjection(GetEntity()->GetScene()->GetWindow()->Width, GetEntity()->GetScene()->GetWindow()->Height);
    m_Renderer.Render(model, view, projection);
}
