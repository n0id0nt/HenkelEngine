#pragma once

#include "ECS/Registry.h"
#include "UI/UIArea.h"

class UISystem
{
public:
	UISystem(Registry* registry) : m_registry(registry)
	{
	}

	void Update();

private:

	void UpdateUIAreas(UIArea* area);

	Registry* m_registry = nullptr;
};

