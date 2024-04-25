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
	std::unique_ptr<UIArea> m_rootArea;
};