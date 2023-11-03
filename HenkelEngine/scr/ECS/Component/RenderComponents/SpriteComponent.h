#pragma once

#include "Resourse\TileSheet.h"

struct SpriteComponent
{
	SpriteComponent(TileSheet tileSheet, unsigned int index) : tileSheet(tileSheet), index(index) {}

	TileSheet tileSheet;
	unsigned int index;
};

