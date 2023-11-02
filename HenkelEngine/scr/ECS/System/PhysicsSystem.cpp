#include "PhysicsSystem.h"
#include "ECS/Component/PhysicsBodyComponents/PhysicsBodyComponent.h"
#include "ECS/Component/PhysicsBodyComponents/StaticBodyComponent.h"
#include "ECS/Component/PhysicsBodyComponents/TileMapCollisionBodyComponent.h"
#include "ECS/Component/TransformComponent.h"
#include "opengl\DebugRenderer.h"
#include <iostream>

PhysicsSystem::PhysicsSystem(Registry* registry, Engine* engine) 
	: m_registry(registry), m_engine(engine)
{
}

void PhysicsSystem::Update(PhysicsWorld* world)
{
	world->Step();
	world->ClearForces();

	//Draw debug shapes
	{
		auto view = m_registry->GetEntitiesWithComponents<StaticBodyComponent, TransformComponent>();
		for (auto& entity : view)
		{
			auto* staticBody = m_registry->GetComponent<StaticBodyComponent>(entity);
			//auto staticBody = view.get<StaticBodyComponent>(entity);
			DebugRenderer::DrawRectangle(glm::vec3{staticBody->GetPosition(), 0.f}, 16.f, 16.f, { 0.f, 0.5f, 0.8f });
		}
	}
	{
		auto view = m_registry->GetEntitiesWithComponents<TileMapCollisionBodyComponent, TransformComponent>();
		for (auto& entity : view)
		{
			auto* transform = m_registry->GetComponent<TransformComponent>(entity);
			auto* tileMap = m_registry->GetComponent<TileMapCollisionBodyComponent>(entity);
			auto positions = tileMap->tilePositions();
			for (auto& position : positions)
				DebugRenderer::DrawRectangle(glm::vec3{position, 0.f}, 16.f, 16.f, { 0.8f, 0.5f, 0.f });
		}
	}
	{
		auto view = m_registry->GetEntitiesWithComponents<PhysicsBodyComponent, TransformComponent>();
		for (auto& entity : view)
		{
			//std::cout << (int)entity << std::endl;
			auto* transform = m_registry->GetComponent<TransformComponent>(entity);
			auto* physicsBody = m_registry->GetComponent<PhysicsBodyComponent>(entity);
			
			transform->SetWorldPosition(physicsBody->GetPosition());
			
			DebugRenderer::DrawRectangle(glm::vec3{physicsBody->GetPosition(), 0.f}, 16.f, 16.f);
		}
	}
}
