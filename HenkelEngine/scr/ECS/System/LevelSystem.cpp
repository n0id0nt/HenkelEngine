#include "LevelSystem.h"
#include "ECS\Component\TransformComponent.h"
#include "ECS\Component\LevelComponent.h"
#include "opengl\DebugRenderer.h"

#ifdef _DEBUG
void LevelSystem::Update(Registry* registry)
{
	auto view = registry->GetEntitiesWithComponents<LevelComponent, TransformComponent>();
	for (auto& entity : view)
	{
		auto* transform = registry->GetComponent<TransformComponent>(entity);
		auto* level = registry->GetComponent<LevelComponent>(entity);
		//auto staticBody = view.get<StaticBodyComponent>(entity);
		glm::ivec2 levelSize = level->size;
		glm::vec2 worldPos = transform->GetWorldPosition();

		DebugRenderer::DrawRectangle(glm::vec3(worldPos.x + levelSize.x / 2.0f, worldPos.y + levelSize.y / 2.0f, 0.0f), levelSize.x, levelSize.y, {0.9f, 0.2f, 0.1f});
	}
}
#endif // _DEBUG
