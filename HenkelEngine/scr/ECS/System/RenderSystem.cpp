#include "RenderSystem.h"
#include "ECS/Component/RenderComponents/SpriteComponent.h"
#include "ECS/Component/RenderComponents/TileMapComponent.h"
#include "ECS/Component/RenderComponents/MaterialComponent.h"
#include "ECS/Component/TransformComponent.h"
#include "Engine.h"
#include "Scene.h"

void RenderSystem::Update()
{
	auto view = m_registry->GetEntitiesWithComponents<RenderComponent, TransformComponent, MaterialComponent>();
	for (auto& entity : view)
	{
		auto* renderComponent = m_registry->GetComponent<RenderComponent>(entity);
		auto* transformComponent = m_registry->GetComponent<TransformComponent>(entity);
		auto* materialComponent = m_registry->GetComponent<MaterialComponent>(entity);
		auto* tilemapComponent = m_registry->GetComponent<TileMapComponent>(entity);
		auto* spriteComponent = m_registry->GetComponent<SpriteComponent>(entity);

		ASSERT(tilemapComponent || spriteComponent);

		if (tilemapComponent)
		{
			auto tileVertices = tilemapComponent->GetTileVertices();
			for (auto& vertice : tileVertices)
			{
				renderComponent->AddQuadToBatch(vertice[0], vertice[1], vertice[2]);
			}
		}

		if (spriteComponent)
		{
			renderComponent->SetQuadUVs(spriteComponent->tileSheet.GetSpriteRectAtIndex(spriteComponent->index));
		}

		glm::mat4 projection = Engine::GetInstance()->GetCurrentScene()->GetCamera()->CalculateProjection((float)Engine::GetInstance()->GetWindow()->GetWidth(), (float)Engine::GetInstance()->GetWindow()->GetHeight());
		glm::mat4 view = Engine::GetInstance()->GetCurrentScene()->GetCamera()->GetViewMatrix();
		glm::mat4 model = transformComponent->GetWorldMatrix();
		materialComponent->Bind(model, view, projection);
		renderComponent->Render();
		materialComponent->Unbind();
	}
}
