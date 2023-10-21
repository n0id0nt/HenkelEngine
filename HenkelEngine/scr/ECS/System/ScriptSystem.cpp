#include "ScriptSystem.h"
#include "ECS\Component\PlayerMovementComponent.h"

ScriptSystem::ScriptSystem(entt::registry* registry) : m_registry(registry)
{
}

void ScriptSystem::Update(float deltaTime)
{
	auto view = m_registry->view<PlayerMovementComponent>();
	for (auto& entity : view)
	{
		auto& transform = view.get<PlayerMovementComponent>(entity);
		transform.Update(deltaTime);
	}
}
