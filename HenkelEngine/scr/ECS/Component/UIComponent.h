#pragma once

#include "UI/UIArea.h"
#include <sol\sol.hpp>
#include <memory>

class UIComponent
{
public:
	UIComponent();

	UIArea* GetRootArea();
	UIArea* FindArea(const std::string& name);

	static void LUABind(sol::state& lua);

private:

	UIArea* RecursiveFindArea(UIArea* area, const std::string& name);

	std::unique_ptr<UIArea> m_rootArea;
};