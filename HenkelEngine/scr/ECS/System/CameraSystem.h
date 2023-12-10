#pragma once
#include <ECS\Registry.h>
#include <Camera.h>

class CameraSystem
{
public:
	CameraSystem(Registry* registry);

	void Update(Camera* camera);

	glm::vec3 CalculateSmoothedPosition(CameraComponent* cameraComponent, const glm::vec3& cameraComponentPos, Camera* camera);
	glm::vec3 TargetPosition(CameraComponent* cameraComponent, const glm::vec3& cameraComponentPos, Camera* camera);

private:

	Registry* m_registry = nullptr;
};

