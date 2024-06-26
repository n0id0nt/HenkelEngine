#include "CameraSystem.h"
#include "ECS\Component\CameraComponent.h"
#include "ECS\Component\TransformComponent.h"
#include "opengl\DebugRenderer.h"
#include "Engine.h"
#include "HelperFunctions.h"

void CameraSystem::Update(Registry* registry, Camera* camera)
{
	auto view = registry->GetEntitiesWithComponents<CameraComponent, TransformComponent>();
	for (auto& entity : view)
	{
		auto* cameraComponent = registry->GetComponent<CameraComponent>(entity);
		if (cameraComponent->IsActiveCamera())
		{
			camera->SetPosition(m_previousCameraPosition);
			auto* transformComponent = registry->GetComponent<TransformComponent>(entity);
			glm::vec cameraComponentPos = transformComponent->GetWorldPosition();

			glm::vec3 newPos = cameraComponent->IsPositionForced() ? TargetPosition(cameraComponent, cameraComponentPos, camera) 
				: CalculateSmoothedPosition(cameraComponent, cameraComponentPos, camera);
			cameraComponent->SetForcePosition(false);

			camera->SetPosition(newPos);
			camera->SetZoom(cameraComponent->GetZoom());
			camera->SetAngle(cameraComponent->GetAngle());
			m_previousCameraPosition = newPos; 

			// exclude shake from previous position
			CalculateCameraShake(cameraComponent, camera);

			if (cameraComponent->debugLines)
			{
				DebugRenderer::DrawScreenLine(glm::vec3{cameraComponent->GetOffset().x + cameraComponent->GetDeadZone().x, -1.f, 0.f}, glm::vec3{cameraComponent->GetOffset().x + cameraComponent->GetDeadZone().x, 1.f, 0.f}, glm::vec3{0.1f, 0.4f, 0.8f});
				DebugRenderer::DrawScreenLine(glm::vec3{cameraComponent->GetOffset().x - cameraComponent->GetDeadZone().x, -1.f, 0.f}, glm::vec3{cameraComponent->GetOffset().x - cameraComponent->GetDeadZone().x, 1.f, 0.f}, glm::vec3{0.1f, 0.4f, 0.8f});
				DebugRenderer::DrawScreenLine(glm::vec3{-1.f, cameraComponent->GetOffset().y + cameraComponent->GetDeadZone().y, 0.f}, glm::vec3{1.f, cameraComponent->GetOffset().y + cameraComponent->GetDeadZone().y, 0.f}, glm::vec3{0.1f, 0.4f, 0.8f});
				DebugRenderer::DrawScreenLine(glm::vec3{-1.f, cameraComponent->GetOffset().y - cameraComponent->GetDeadZone().y, 0.f}, glm::vec3{1.f, cameraComponent->GetOffset().y - cameraComponent->GetDeadZone().y, 0.f}, glm::vec3{0.1f, 0.4f, 0.8f});
			}
		}
	}
}

glm::vec3 CameraSystem::CalculateSmoothedPosition(CameraComponent* cameraComponent, const glm::vec3& cameraComponentPos, Camera* camera)
{
	float deltaTime = Engine::GetInstance()->GetTime()->GetDeltaTime();

	glm::vec2 cameraComponentScreenPos = camera->WorldPosToScreenPos(cameraComponentPos);
	glm::vec2 newPos{};
	// x
	{
		if (cameraComponent->GetOffset().x + cameraComponent->GetDeadZone().x < cameraComponentScreenPos.x)
		{
			newPos.x = glm::mix(0.f, cameraComponentScreenPos.x - cameraComponent->GetOffset().x - cameraComponent->GetDeadZone().x, cameraComponent->GetDamping().x * deltaTime);
		}
		else if (cameraComponent->GetOffset().x - cameraComponent->GetDeadZone().x > cameraComponentScreenPos.x)
		{
			newPos.x = glm::mix(0.f, cameraComponentScreenPos.x - cameraComponent->GetOffset().x + cameraComponent->GetDeadZone().x, cameraComponent->GetDamping().x * deltaTime);
		}
	}
	// y 
	{
		if (cameraComponent->GetOffset().y + cameraComponent->GetDeadZone().y < cameraComponentScreenPos.y)
		{
			newPos.y = glm::mix(0.f, cameraComponentScreenPos.y - cameraComponent->GetOffset().y - cameraComponent->GetDeadZone().y, cameraComponent->GetDamping().y * deltaTime);
		}
		else if (cameraComponent->GetOffset().y - cameraComponent->GetDeadZone().y > cameraComponentScreenPos.y)
		{
			newPos.y = glm::mix(0.f, cameraComponentScreenPos.y - cameraComponent->GetOffset().y + cameraComponent->GetDeadZone().y, cameraComponent->GetDamping().y * deltaTime);
		}
	}

	return glm::vec3{camera->ScreenPosToWorldPos(newPos), 0.f};
}

glm::vec3 CameraSystem::TargetPosition(CameraComponent* cameraComponent, const glm::vec3& cameraComponentPos, Camera* camera)
{
	glm::vec2 cameraComponentScreenPos = camera->WorldPosToScreenPos(cameraComponentPos);
	return glm::vec3{camera->ScreenPosToWorldPos(cameraComponentScreenPos - cameraComponent->GetOffset()), 0.f};
}

void CameraSystem::CalculateCameraShake(CameraComponent* cameraComponent, Camera* camera)
{
	float trauma = cameraComponent->GetTrauma();
	if (trauma > 0.f)
	{
		const int seed = 0;
		float shake = std::pow(trauma, 2.f);
		float time = Engine::GetInstance()->GetTime()->GetTime();
		float angle = cameraComponent->GetMaxAngle() * shake * HenkelEngine::PerlinNoise(seed, time / cameraComponent->GetShakeAmplitude());
		float xOffset = cameraComponent->GetMaxOffset() * shake * HenkelEngine::PerlinNoise(seed + 1, time / cameraComponent->GetShakeAmplitude());
		float yOffset = cameraComponent->GetMaxOffset() * shake * HenkelEngine::PerlinNoise(seed + 2, time / cameraComponent->GetShakeAmplitude());

		float deltaTime = Engine::GetInstance()->GetTime()->GetDeltaTime();
		cameraComponent->SetTrauma(trauma - deltaTime / cameraComponent->GetTraumaTime());
		camera->SetPosition(camera->GetPosition() + glm::vec3(xOffset, yOffset, 0.f));
		camera->SetAngle(camera->GetAngle() + angle);
	}
	else
	{
		cameraComponent->ResetTrauma();
	}
}
