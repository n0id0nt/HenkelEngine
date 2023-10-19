#include "RenderComponent.h"
#include "Camera.h"
#include "ECS/Entity/Entity.h"
#include "Scene.h"
#include "Window.h"
#include "opengl/Texture.h"
#include "Engine.h"

RenderComponent::RenderComponent(Entity* entity, Engine* engine, unsigned int QuadCound, std::string texture)
    : Component(entity), m_Renderer(new Material(texture, "res/shaders/sprite.vert", "res/shaders/sprite.frag"), QuadCound), m_engine(engine)
{
}

void RenderComponent::Render()
{
    Camera* camera= GetEntity()->GetScene()->GetCamera();
    glm::mat4 model = GetEntity()->GetTransform()->GetWorldMatrix();
    glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 projection = camera->CalculateProjection(m_engine->GetWindow()->GetWidth(), m_engine->GetWindow()->GetHeight());
    m_Renderer.Render(model, view, projection);
}
