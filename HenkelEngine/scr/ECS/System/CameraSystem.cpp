#include "CameraSystem.h"
#include <ECS\Component\CameraComponent.h>
#include <ECS\Component\TransformComponent.h>
#include "opengl\DebugRenderer.h"
#include <Engine.h>

CameraSystem::CameraSystem(Registry* registry) : m_registry(registry)
{
}

void CameraSystem::Update(Camera* camera)
{
	float deltaTime = Engine::GetInstance()->GetTime()->GetDeltaTime();
	auto view = m_registry->GetEntitiesWithComponents<CameraComponent, TransformComponent>();
	for (auto& entity : view)
	{
		auto* cameraComponent = m_registry->GetComponent<CameraComponent>(entity);
		if (cameraComponent->IsActiveCamera())
		{
			auto* transformComponent = m_registry->GetComponent<TransformComponent>(entity);
			glm::vec3 cameraComponentPos = transformComponent->GetWorldPosition();
			glm::vec2 cameraComponentScreenPos = camera->WorldPosToScreenPos(cameraComponentPos);

			glm::vec2 newPos{};
			// x
			{
				if (cameraComponent->GetOffset().x + cameraComponent->GetSoftZone().x < cameraComponentScreenPos.x)
				{
					newPos.x = glm::mix(cameraComponent->GetOffset().x + cameraComponent->GetDeadZone().x, cameraComponentScreenPos.x, cameraComponent->GetHardDamping().x * deltaTime);
				}
				else if (cameraComponent->GetOffset().x - cameraComponent->GetSoftZone().x > cameraComponentScreenPos.x)
				{
					newPos.x = glm::mix(cameraComponent->GetOffset().x - cameraComponent->GetDeadZone().x, cameraComponentScreenPos.x, cameraComponent->GetHardDamping().x * deltaTime);
				}
				else if (cameraComponent->GetOffset().x + cameraComponent->GetDeadZone().x < cameraComponentScreenPos.x)
				{
					newPos.x = glm::mix(cameraComponent->GetOffset().x + cameraComponent->GetDeadZone().x, cameraComponentScreenPos.x, cameraComponent->GetSoftDamping().x * deltaTime);
				}
				else if (cameraComponent->GetOffset().x - cameraComponent->GetDeadZone().x > cameraComponentScreenPos.x)
				{
					newPos.x = glm::mix(cameraComponent->GetOffset().x - cameraComponent->GetDeadZone().x, cameraComponentScreenPos.x, cameraComponent->GetSoftDamping().x * deltaTime);
				}
			}
			// y 
			{
				if (cameraComponent->GetOffset().y + cameraComponent->GetSoftZone().y > cameraComponentScreenPos.y)
				{
					newPos.y = glm::mix(cameraComponent->GetOffset().y + cameraComponent->GetDeadZone().y, cameraComponentScreenPos.y, cameraComponent->GetHardDamping().y * deltaTime);
				}
				else if (cameraComponent->GetOffset().y - cameraComponent->GetSoftZone().y < cameraComponentScreenPos.y)
				{
					newPos.y = glm::mix(cameraComponent->GetOffset().y - cameraComponent->GetDeadZone().y, cameraComponentScreenPos.y, cameraComponent->GetHardDamping().y * deltaTime);
				}
				else if (cameraComponent->GetOffset().y + cameraComponent->GetDeadZone().y > cameraComponentScreenPos.y)
				{
					newPos.y = glm::mix(cameraComponent->GetOffset().y + cameraComponent->GetDeadZone().y, cameraComponentScreenPos.y, cameraComponent->GetSoftDamping().y * deltaTime);
				}
				else if (cameraComponent->GetOffset().y - cameraComponent->GetDeadZone().y < cameraComponentScreenPos.y)
				{
					newPos.y = glm::mix(cameraComponent->GetOffset().y - cameraComponent->GetDeadZone().y, cameraComponentScreenPos.y, cameraComponent->GetSoftDamping().y * deltaTime);
				}
			}




			//glm::vec3 newPos = glm::mix(camera->GetPosition(), cameraComponentPos, 0.2f);
			
			
			camera->SetPosition(glm::vec3{camera->ScreenPosToWorldPos(newPos), 0.f});
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
