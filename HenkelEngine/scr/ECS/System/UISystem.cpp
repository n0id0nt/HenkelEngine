#include "UISystem.h"
#include "ECS/Component/UIComponent.h"

void UISystem::Update()
{
	auto uiComponents = m_registry->GetAllComponents<UIComponent>();
	for (auto& uiComponent : uiComponents)
	{
		UpdateUIAreas(uiComponent->GetRootArea());
	}
}

void UISystem::UpdateUIAreas(UIArea* area)
{
	area->Update();
	for (auto& area : area->GetChildren())
	{
		UpdateUIAreas(area);
	}
}
