#pragma once

#include "opengl/Renderer.h"
#include "../Component.h"

class Engine;

class RenderComponent : public Component
{
public:
	RenderComponent(Entity* entity, Engine* engine, unsigned int QuadCound = 1, std::string texture = "res/images/CubeFace.png");
	
	virtual void Render();

protected:
	Renderer m_Renderer;
	Engine* m_engine;
};