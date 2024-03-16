#pragma once

#include "ECS/Registry.h"
#include "UI/UIArea.h"
#include "opengl/BatchRenderer.h"

class UIRenderSystem
{
public:
	UIRenderSystem(Registry* registry);
	~UIRenderSystem();

	void Update();

private:

	void RenderUIAreas(UIArea* area, BatchRenderer* batchRenderer);

	Registry* m_registry = nullptr;

	BatchRenderer m_batchRenderer;

	static constexpr const char* VERTEX_SHADER = "res/shaders/UI.vert";
	static constexpr const char* FREGMENT_SHADER = "res/shaders/UI.frag";

	static constexpr const int BATCH_QUAD_SIZE = 6000;
};

