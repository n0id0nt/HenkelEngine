#pragma once

#include "Resourse\TileSheet.h"
#include "sol\sol.hpp"

struct SpriteComponent
{
	SpriteComponent(TileSheet tileSheet, unsigned int index, float xOffset = 0.f, float yOffset = 0.f) 
		: tileSheet(tileSheet), index(index), flipped(false), xOffset(xOffset), yOffset(yOffset) {}

	float xOffset, yOffset;
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

