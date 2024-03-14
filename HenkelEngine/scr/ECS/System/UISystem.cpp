#include "UISystem.h"
#include "ECS/Component/UIComponent.h"

void UISystem::Update()
{
	auto view = m_registry->GetEntitiesWithComponents<UIComponent>();
	for (auto& entity : view)
	{
		auto view = m_registry->GetEntitiesWithComponent<UIComponent>();
		for (auto& entity : view)
		{
			auto* uiComponent = m_registry->GetComponent<UIComponent>(entity);
			UpdateUIAreas(uiComponent->rootArea.get());
		}
	}
}

void UISystem::UpdateUIAreas(UIArea* area)
{
	area->Update();
	for (auto& area : area->GetChildren())
	{
		UpdateUIAreas(&area);
	}
}
