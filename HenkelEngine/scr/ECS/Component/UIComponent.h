#pragma once

#include "UI/UIArea.h"
#include <sol\sol.hpp>
#include <memory>

struct UIComponent
{
	std::unique_ptr<UIArea> rootArea;

	static void LUABind(sol::state& lua)
	{
		lua.new_usertype<UIComponent>("sprite",
			"rootArea", &UIComponent::rootArea
		);
	}
};