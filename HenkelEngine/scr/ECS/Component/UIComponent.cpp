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

void UIComponent::LUABind(sol::state& lua)
{
	lua.new_usertype<UIComponent>("uiComponent",
		"rootArea", &UIComponent::GetRootArea
	);
}