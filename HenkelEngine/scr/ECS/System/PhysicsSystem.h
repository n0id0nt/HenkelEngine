#pragma once

#include "Physics\PhysicsWorld.h"
#include "ECS\Registry.h"
#include <Physics\ContactListener.h>

namespace PhysicsSystem
{
	void Update(Registry* registry, PhysicsWorld* world);
};
