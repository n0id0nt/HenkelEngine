#include "UISystem.h"
#include "ECS/Component/UIComponent.h"

void RecursiveUpdateUIAreas(UIArea* area)
{
	area->Update();
	for (auto& area : area->GetChildren())
	{
		RecursiveUpdateUIAreas(area);
	}
}

void UISystem::Update(Registry* registry)
{
	auto uiComponents = registry->GetAllComponents<UIComponent>();
	for (auto& uiComponent : uiComponents)
	{
		RecursiveUpdateUIAreas(uiComponent->GetRootArea());
	}
}
