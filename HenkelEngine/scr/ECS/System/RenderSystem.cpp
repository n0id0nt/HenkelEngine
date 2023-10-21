#include "RenderSystem.h"
#include "ECS/Component/RenderComponents/SpriteComponent.h"
#include "ECS/Component/RenderComponents/TileMapComponent.h"
#include "ECS/Component/TransformComponent.h"
#include "Engine.h"
#include "Scene.h"

void RenderSystem::Update()
{
	auto view = m_registry->view<RenderComponent, TransformComponent>();
	for (auto& entity : view)
	{
		auto& renderComponent = view.get<RenderComponent>(entity);
		auto& transformComponent = view.get<TransformComponent>(entity);
		auto* tilemapComponent = m_registry->try_get<TileMapComponent>(entity);
		auto* spriteComponent = m_registry->try_get<SpriteComponent>(entity);

		ASSERT(tilemapComponent || spriteComponent);

		if (tilemapComponent)
		{
			auto tileVertices = tilemapComponent->GetTileVertices();
			for (auto& vertice : tileVertices)
			{
				renderComponent.AddQuadToBatch(vertice[0], vertice[1], vertice[2]);
			}
		}

		if (spriteComponent)
		{
			renderComponent.SetQuadUVs(spriteComponent->tileSheet.GetSpriteRectAtIndex(spriteComponent->index));
		}

		glm::mat4 projection = m_engine->GetCurrentScene()->GetCamera()->CalculateProjection(m_engine->GetWindow()->GetWidth(), m_engine->GetWindow()->GetHeight());
		glm::mat4 view = m_engine->GetCurrentScene()->GetCamera()->GetViewMatrix();
		glm::mat4 model = transformComponent.GetWorldMatrix();
		renderComponent.Render(model, view, projection);
	}
}
