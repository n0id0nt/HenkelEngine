#pragma once

#include "sol\sol.hpp"
#include "LUABindable.h"
#include <string>
#include "ECS\Entity\Entity.h"
#include "ECS\Registry\Registry.h"

namespace ScriptSystem
{
	void Update(Registry* registry, sol::state& lua);
};

