#pragma once

#include "Panel.h"
#include "../opengl/FrameBuffer.h"
#include <memory>

class ScenePanel :
	public Panel
{
public:
	ScenePanel(std::string name, Scene* scene)
		: Panel(name, scene), m_frameBuffer(std::make_unique<FrameBuffer>()) 
	{}

	void Render() override;

	void OnResize() override;

private:

	std::unique_ptr<FrameBuffer> m_frameBuffer;
};

