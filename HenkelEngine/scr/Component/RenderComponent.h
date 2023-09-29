#pragma once

#include "../opengl/Renderer.h"
#include "Component.h"

class RenderComponent : public Component
{
public:
	RenderComponent(Entity* entity);
	
	void Render();

private:
	Renderer m_Renderer;
};