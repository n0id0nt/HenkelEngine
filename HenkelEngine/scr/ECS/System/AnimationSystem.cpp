#include "AnimationSystem.h"
#include "ECS/Component/RenderComponents/SpriteComponent.h"
#include "ECS/Component/SpriteAnimationComponent.h"

void AnimationSystem::Update(Registry* registry)
{
	auto view = registry->GetEntitiesWithComponents<SpriteComponent, SpriteAnimationComponent>();
	for (auto& entity : view)
	{
		auto* sprite = registry->GetComponent<SpriteComponent>(entity);
		auto* animation = registry->GetComponent<SpriteAnimationComponent>(entity);
		int frame = animation->GetCurrentFrame();
		sprite->index = frame;
	}
}
