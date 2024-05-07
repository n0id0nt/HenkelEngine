#pragma once

#include "UI/UIArea.h"
#include <sol\sol.hpp>
#include <memory>

class UIComponent
{
public:
	UIComponent();

	UIArea* GetRootArea();
	
	template <typename UIType>
	UIType* FindArea(const std::string& name)
	{
		UIArea* area = RecursiveFindArea(m_rootArea.get(), name);
		return dynamic_cast<UIType*>(area);
	}

	static void LUABind(sol::state& area);

private:

	UIArea* RecursiveFindArea(UIArea* area, const std::string& name);

	std::unique_ptr<UIArea> m_rootArea;
};