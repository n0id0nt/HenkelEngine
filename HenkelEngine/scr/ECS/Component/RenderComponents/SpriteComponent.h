#pragma once

#include "Resourse\TileSheet.h"
#include "sol\sol.hpp"

struct SpriteComponent
{
	SpriteComponent(TileSheet tileSheet, unsigned int index) : tileSheet(tileSheet), index(index), flipped(false) {}

	TileSheet tileSheet;
	unsigned int index;
	bool flipped;

	static void LUABind(sol::state& lua)
	{
		lua.new_usertype<SpriteComponent>("sprite",
			"flipped", &SpriteComponent::flipped
		);
	}
};

