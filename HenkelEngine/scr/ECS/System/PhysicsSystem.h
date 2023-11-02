#pragma once

#include "Physics\PhysicsWorld.h"
#include "ECS\Registry.h"


class Engine;

class PhysicsSystem
{
public:
	PhysicsSystem(Registry* registry, Engine* engine);

	void Update(PhysicsWorld* world);

private:

	Registry* m_registry = nullptr;
	Engine* m_engine = nullptr;
};

