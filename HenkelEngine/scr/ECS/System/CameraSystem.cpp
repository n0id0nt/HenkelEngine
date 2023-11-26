#include "CameraSystem.h"
#include <ECS\Component\CameraComponent.h>
#include <ECS\Component\TransformComponent.h>

CameraSystem::CameraSystem(Registry* registry) : m_registry(registry)
{
}

void CameraSystem::Update(Camera* camera)
{
	auto view = m_registry->GetEntitiesWithComponents<CameraComponent, TransformComponent>();
	for (auto& entity : view)
	{
		auto* transform = m_registry->GetComponent<TransformComponent>(entity);
		camera->SetPosition(transform->GetWorldPosition());
	}
}
