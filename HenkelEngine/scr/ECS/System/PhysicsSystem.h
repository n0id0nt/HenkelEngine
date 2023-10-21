#pragma once

#include "entt.hpp"
#include "Physics\PhysicsWorld.h"

class Engine;

class PhysicsSystem
{
public:
	PhysicsSystem(entt::registry* registry, Engine* engine);

	void Update(PhysicsWorld* world);

private:

	entt::registry* m_registry = nullptr;
	Engine* m_engine = nullptr;
};

