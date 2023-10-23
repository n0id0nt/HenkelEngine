#pragma once

#include "sol/sol.hpp"

class LUABindable
{
public:
	  virtual void LUABind(sol::state& lua) = 0;
};