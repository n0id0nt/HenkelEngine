#include "UISystem.h"
#include "ECS/Component/UIComponent.h"

void UISystem::Update()
{
	auto view = m_registry->GetEntitiesWithComponent<UIComponent>();
	for (auto& entity : view)
	{
		auto* uiComponent = m_registry->GetComponent<UIComponent>(entity);
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
