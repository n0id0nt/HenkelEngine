#include "LevelSystem.h"
#include "ECS\Component\TransformComponent.h"
#include "ECS\Component\LevelComponent.h"
#include "opengl\DebugRenderer.h"
#include "FileParser.h"

void LevelSystem::LoadLevel(World* world, Entity* levelEntity)
{
	FileParser::LoadLevel(world, levelEntity);
}

void LevelSystem::UnloadLevel(World* world, Entity* levelEntity)
{
	auto* levelComponent = levelEntity->GetComponent<LevelComponent>();
	levelComponent->loaded = false;
	for (auto childEntity : levelEntity->GetChildren())
	{
		childEntity->Delete();
		//world->DeleteEntity(childEntity);
	}
}

Entity* LevelSystem::FindLevelWithPosition(World* world, Registry* registry, const glm::vec2& position)
{
	auto view = registry->GetEntitiesWithComponents<LevelComponent, TransformComponent>();
	for (auto entity : view)
	{
		auto* transform = registry->GetComponent<TransformComponent>(entity);
		auto* level = registry->GetComponent<LevelComponent>(entity);
		glm::ivec2 size = level->size;
		glm::vec2 levelPosition = transform->GetWorldPosition2();
		if (position.x >= levelPosition.x &&			// right of the left edge AND
			position.x <= levelPosition.x + size.x &&   // left of the right edge AND
			position.y >= levelPosition.y &&			// below the top AND
			position.y <= levelPosition.y + size.y)		// above the bottom
		{   
			return world->GetEntityFromEntityID(entity);
		}
	}
	return nullptr;
}

#ifdef _DEBUG
void LevelSystem::Update(Registry* registry)
{
	auto view = registry->GetEntitiesWithComponents<LevelComponent, TransformComponent>();
	for (auto entity : view)
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
