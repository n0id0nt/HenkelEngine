#include "CameraSystem.h"
#include <ECS\Component\CameraComponent.h>
#include <ECS\Component\TransformComponent.h>
#include "opengl\DebugRenderer.h"

CameraSystem::CameraSystem(Registry* registry) : m_registry(registry)
{
}

void CameraSystem::Update(Camera* camera)
{
	auto view = m_registry->GetEntitiesWithComponents<CameraComponent, TransformComponent>();
	for (auto& entity : view)
	{
		auto* cameraComponent = m_registry->GetComponent<CameraComponent>(entity);
		if (cameraComponent->IsActiveCamera())
		{
			auto* transformComponent = m_registry->GetComponent<TransformComponent>(entity);

			glm::vec2 testPoint = camera->WorldPosToScreenPos(glm::vec2());

			camera->SetPosition(transformComponent->GetWorldPosition());
			camera->SetZoom(cameraComponent->GetZoom());

			if (cameraComponent->debugLines)
			{
				DebugRenderer::DrawScreenLine(glm::vec3{cameraComponent->GetOffset().x + cameraComponent->GetDeadZone().x, -1.f, 0.f}, glm::vec3{cameraComponent->GetOffset().x + cameraComponent->GetDeadZone().x, 1.f, 0.f}, glm::vec3{0.1f, 0.4f, 0.8f});
				DebugRenderer::DrawScreenLine(glm::vec3{cameraComponent->GetOffset().x - cameraComponent->GetDeadZone().x, -1.f, 0.f}, glm::vec3{cameraComponent->GetOffset().x - cameraComponent->GetDeadZone().x, 1.f, 0.f}, glm::vec3{0.1f, 0.4f, 0.8f});
				DebugRenderer::DrawScreenLine(glm::vec3{-1.f, cameraComponent->GetOffset().y + cameraComponent->GetDeadZone().y, 0.f}, glm::vec3{1.f, cameraComponent->GetOffset().y + cameraComponent->GetDeadZone().y, 0.f}, glm::vec3{0.1f, 0.4f, 0.8f});
				DebugRenderer::DrawScreenLine(glm::vec3{-1.f, cameraComponent->GetOffset().y - cameraComponent->GetDeadZone().y, 0.f}, glm::vec3{1.f, cameraComponent->GetOffset().y - cameraComponent->GetDeadZone().y, 0.f}, glm::vec3{0.1f, 0.4f, 0.8f});
				DebugRenderer::DrawScreenRectangle(glm::vec3{cameraComponent->GetOffset(), 0.f}, cameraComponent->GetSoftZone().x * 2.f, cameraComponent->GetSoftZone().y * 2.f, glm::vec3{1.0f, 0.1f, 0.1f});
			}
		}
	}
}
