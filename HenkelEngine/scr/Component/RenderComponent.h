#pragma once

#include "../opengl/Renderer.h"
#include "Component.h"

class RenderComponent : public Component
{
public:
	RenderComponent(Entity* entity, bool stretchToImageSize, unsigned int QuadCound = 1, std::string texture = "res/images/CubeFace.png");
	
	virtual void Render();

protected:
	Renderer m_Renderer;
};