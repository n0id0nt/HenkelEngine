#include "UIComponent.h"
#include "UI/UIText.h"
#include <memory>

UIComponent::UIComponent()
{
	m_rootArea = std::make_unique<UIArea>();
}

UIArea* UIComponent::GetRootArea()
{
	return m_rootArea.get();
}

void UIComponent::LUABind(sol::state& lua)
{
	lua.new_usertype<UIComponent>("uiComponent",
		"rootArea", &UIComponent::GetRootArea,
		"findArea", &UIComponent::FindArea<UIArea>,
		"findText", &UIComponent::FindArea<UIText>
	);
}

UIArea* UIComponent::RecursiveFindArea(UIArea* area, const std::string& name)
{
	for (UIArea* child : area->GetChildren())
	{
		if (child->GetName() == name)
		{
			return child;
		}
		UIArea* returnValue = RecursiveFindArea(child, name);
		if (returnValue)
		{
			return returnValue;
		}
	}
	return nullptr;
}
