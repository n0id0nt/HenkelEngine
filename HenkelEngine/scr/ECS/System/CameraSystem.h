#pragma once
#include <ECS\Registry.h>
#include "Camera.h"

namespace CameraSystem
{
	void Update(Registry* registry, Camera* camera);

	glm::vec3 CalculateSmoothedPosition(CameraComponent* cameraComponent, const glm::vec3& cameraComponentPos, Camera* camera);
	glm::vec3 TargetPosition(CameraComponent* cameraComponent, const glm::vec3& cameraComponentPos, Camera* camera);
	void CalculateCameraShake(CameraComponent* cameraComponent, Camera* camera);

	static glm::vec3 m_previousCameraPosition;
	static float m_previousCameraAngle;
};

