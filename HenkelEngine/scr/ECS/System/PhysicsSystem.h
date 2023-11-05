#pragma once

#include "Physics\PhysicsWorld.h"
#include "ECS\Registry.h"

class PhysicsSystem
{
public:
	PhysicsSystem(Registry* registry);

	void Update(PhysicsWorld* world);

private:

	Registry* m_registry = nullptr;
};

