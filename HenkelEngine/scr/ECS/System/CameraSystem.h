#pragma once
#include <ECS\Registry.h>
#include <Camera.h>

class CameraSystem
{
public:
	CameraSystem(Registry* registry);

	void Update(Camera* camera);

private:

	Registry* m_registry = nullptr;
};

