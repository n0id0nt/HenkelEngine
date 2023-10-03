#pragma once
#include "Component.h"
#include "RenderComponent.h"
#include "..\Resourse\TileSheet.h"

class SpriteComponent : public RenderComponent
{
public:
	SpriteComponent(Entity* entity, TileSheet tileSheet, unsigned int index);
	
	void Update() override;
private:

	TileSheet m_tilesheet;
	unsigned int m_index;
};

