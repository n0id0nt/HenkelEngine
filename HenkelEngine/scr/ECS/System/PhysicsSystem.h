#pragma once

#include "Physics\PhysicsWorld.h"
#include "ECS\Registry.h"
#include <Physics\ContactListener.h>

class PhysicsSystem
{
public:
	PhysicsSystem(Registry* registry);

	void Update(PhysicsWorld* world);

	ContactListener* GetContactListener();

private:

	Registry* m_registry = nullptr;

	std::unique_ptr<ContactListener> m_contactListenter;
};
