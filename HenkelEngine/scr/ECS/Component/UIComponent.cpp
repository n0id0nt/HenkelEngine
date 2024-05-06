#include "UIComponent.h"
#include <memory>

UIComponent::UIComponent()
{
	m_rootArea = std::make_unique<UIArea>();
}

UIArea* UIComponent::GetRootArea()
{
	return m_rootArea.get();
}

UIArea* UIComponent::FindArea(const std::string& name)
{
	return RecursiveFindArea(m_rootArea.get(), name);
}

void UIComponent::LUABind(sol::state& lua)
{
	lua.new_usertype<UIComponent>("uiComponent",
		"rootArea", &UIComponent::GetRootArea,
		"findArea", &UIComponent::FindArea
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
