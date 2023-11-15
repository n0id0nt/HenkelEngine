#include "AnimationSystem.h"
#include "ECS/Component/RenderComponents/SpriteComponent.h"
#include "ECS/Component/SpriteAnimationComponent.h"


AnimationSystem::AnimationSystem(Registry* registry) : m_registry(registry)
{
}

void AnimationSystem::Update()
{
	auto view = m_registry->GetEntitiesWithComponents<SpriteComponent, SpriteAnimationComponent>();
	for (auto& entity : view)
	{
		auto* sprite = m_registry->GetComponent<SpriteComponent>(entity);
		auto* animation = m_registry->GetComponent<SpriteAnimationComponent>(entity);
		int frame = animation->GetCurrentFrame();
		sprite->index = frame;
	}
}
