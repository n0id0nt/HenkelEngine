#pragma once
#include "Component.h"
#include "RenderComponent.h"

class SpriteComponent : public RenderComponent
{
public:
	SpriteComponent(Entity* entity) : RenderComponent(entity, true) {}
	
	void Update() override;
};

