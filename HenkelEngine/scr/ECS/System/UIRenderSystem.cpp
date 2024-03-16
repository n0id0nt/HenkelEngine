#include "UIRenderSystem.h"
#include "ECS/Component/UIComponent.h"
#include "ECS/Component/TransformComponent.h"
#include "Engine.h"

UIRenderSystem::UIRenderSystem(Registry* registry) : m_registry(registry), m_batchRenderer(BATCH_QUAD_SIZE)
{
	Engine::GetInstance()->GetResourcePool()->CreateShader(VERTEX_SHADER, FREGMENT_SHADER);
}

UIRenderSystem::~UIRenderSystem()
{
	Engine::GetInstance()->GetResourcePool()->ReleaseShader(VERTEX_SHADER, FREGMENT_SHADER);
}

void UIRenderSystem::Update()
{
	//TODO check if ui batch is valid

	if (!m_batchRenderer.BatchValid())
	{
		m_batchRenderer.ClearBatch();
		auto view = m_registry->GetEntitiesWithComponent<UIComponent>();
		for (auto& entity : view)
		{
			auto* uiComponent = m_registry->GetComponent<UIComponent>(entity);
			//auto* transformComponent = m_registry->GetComponent<TransformComponent>(entity);

			//glm::mat4 model = transformComponent->GetWorldMatrix();

			RenderUIAreas(uiComponent->GetRootArea(), &m_batchRenderer);
		}
		m_batchRenderer.ValidateBatch();
	}

	Engine::GetInstance()->GetResourcePool()->RetrieveShader(VERTEX_SHADER, FREGMENT_SHADER)->Bind();
	m_batchRenderer.Render();
	Engine::GetInstance()->GetResourcePool()->RetrieveShader(VERTEX_SHADER, FREGMENT_SHADER)->Unbind();
}

void UIRenderSystem::RenderUIAreas(UIArea* area, BatchRenderer* batchRenderer)
{
	area->Render(batchRenderer);
	for (auto& area : area->GetChildren())
	{
		RenderUIAreas(area, batchRenderer);
	}
}
