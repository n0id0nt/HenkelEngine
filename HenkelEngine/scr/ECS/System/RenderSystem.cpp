#include "RenderSystem.h"
#include "ECS/Component/RenderComponents/SpriteComponent.h"
#include "ECS/Component/RenderComponents/TileMapComponent.h"
#include "ECS/Component/RenderComponents/MaterialComponent.h"
#include "ECS/Component/UIComponent.h"
#include "ECS/Component/TransformComponent.h"
#include "Engine.h"
#include "Scene.h"
#include "glm\glm.hpp"
#include <opengl\DebugRenderer.h>

void RenderUIAreas(UIArea* area, BatchRenderer* batchRenderer)
{
	area->Render(batchRenderer);
	for (auto& area : area->GetChildren())
	{
		RenderUIAreas(area, batchRenderer);
	}
}

void RenderSystem::Update()
{
	float width = Engine::GetInstance()->GetWindow()->GetWidth();
	float height = Engine::GetInstance()->GetWindow()->GetHeight();
	Camera* camera = Engine::GetInstance()->GetCurrentScene()->GetCamera();

	auto view = m_registry->GetEntitiesWithComponents<RenderComponent, TransformComponent, MaterialComponent>();
	// TODO add sorting to layers so there are different depths
	for (auto& entity : view)
	{
		auto* renderComponent = m_registry->GetComponent<RenderComponent>(entity);
		auto* transformComponent = m_registry->GetComponent<TransformComponent>(entity);
		auto* materialComponent = m_registry->GetComponent<MaterialComponent>(entity);
		auto* tilemapComponent = m_registry->GetComponent<TileMapComponent>(entity);
		auto* spriteComponent = m_registry->GetComponent<SpriteComponent>(entity);

		if (!(tilemapComponent || spriteComponent)) continue;

		glm::mat4 projection = camera->CalculateProjection(width, height);
		glm::mat4 view = Engine::GetInstance()->GetCurrentScene()->GetCamera()->GetViewMatrix();
		glm::mat4 model = transformComponent->GetWorldMatrix();

		if (tilemapComponent && !renderComponent->BatchValid())
		{
			renderComponent->ClearBatch();
			auto tileVertices = tilemapComponent->GetTileVertices();
			for (auto& vertice : tileVertices)
			{
				renderComponent->AddQuadToBatch(vertice[0], vertice[1], vertice[2]);
			}
			renderComponent->ValidateBatch();
		}

		if (spriteComponent)
		{
			renderComponent->SetQuadUVs(spriteComponent->tileSheet.GetSpriteRectAtIndex(spriteComponent->index), spriteComponent->flipped);
			model = glm::translate(glm::mat4(1.0f), glm::vec3{spriteComponent->xOffset * (spriteComponent->flipped ? -1.0f : 1.0f), spriteComponent->yOffset, 0.0f}) * model;
		}

		materialComponent->Bind(model, view, projection);
		renderComponent->Render();
		materialComponent->Unbind();
	}


	glm::mat4 projection = camera->CalculateProjection((float)Engine::GetInstance()->GetWindow()->GetWidth(), (float)Engine::GetInstance()->GetWindow()->GetHeight());
	glm::mat4 viewMat = camera->GetViewMatrix();
	DebugRenderer::Render(projection * viewMat);

	auto uiView = m_registry->GetEntitiesWithComponents<RenderComponent, TransformComponent, MaterialComponent, UIComponent>();
	// TODO add sorting to layers so there are different depths
	for (auto& entity : uiView)
	{
		auto* renderComponent = m_registry->GetComponent<RenderComponent>(entity);
		auto* transformComponent = m_registry->GetComponent<TransformComponent>(entity);
		auto* materialComponent = m_registry->GetComponent<MaterialComponent>(entity);
		auto* uiComponent = m_registry->GetComponent<UIComponent>(entity);

		glm::mat4 model = transformComponent->GetWorldMatrix();
		glm::mat4 projection = glm::ortho(0.0f, width, height, 0.0f);
		glm::mat4 view = glm::mat4(1.0f);

		if (!renderComponent->BatchValid())
		{
			renderComponent->ClearBatch();
			RenderUIAreas(uiComponent->GetRootArea(), renderComponent->GetBatchRenderer());
			renderComponent->ValidateBatch();
		}

		materialComponent->Bind(model, view, projection);
		renderComponent->Render();
		materialComponent->Unbind();
	}
}
