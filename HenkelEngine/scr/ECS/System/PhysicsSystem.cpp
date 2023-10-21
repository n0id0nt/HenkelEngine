#include "PhysicsSystem.h"
#include "ECS/Component/PhysicsBodyComponents/PhysicsBodyComponent.h"
#include "ECS/Component/PhysicsBodyComponents/StaticBodyComponent.h"
#include "ECS/Component/PhysicsBodyComponents/TileMapCollisionBodyComponent.h"
#include "ECS/Component/TransformComponent.h"
#include "opengl\DebugRenderer.h"

PhysicsSystem::PhysicsSystem(entt::registry* registry, Engine* engine) 
	: m_registry(registry), m_engine(engine)
{
}

void PhysicsSystem::Update(PhysicsWorld* world)
{
	//Draw debug shapes
	{
		auto view = m_registry->view<StaticBodyComponent, TransformComponent>();
		for (auto& entity : view)
		{
			auto& transform = view.get<TransformComponent>(entity);
			//auto staticBody = view.get<StaticBodyComponent>(entity);
			DebugRenderer::DrawRectangle(transform.GetWorldPosition(), 16.f, 16.f);
		}
	}
	{
		auto view = m_registry->view<TileMapCollisionBodyComponent, TransformComponent>();
		for (auto& entity : view)
		{
			auto& transform = view.get<TransformComponent>(entity);
			auto& tileMap = view.get<TileMapCollisionBodyComponent>(entity);
			DebugRenderer::DrawRectangle(transform.GetWorldPosition(), 16.f, 16.f);
		}
	}
	{
		world->Step();
		auto view = m_registry->view<PhysicsBodyComponent, TransformComponent>();
		for (auto& entity : view)
		{
			auto& transform = view.get<TransformComponent>(entity);
			auto& physicsBody = view.get<PhysicsBodyComponent>(entity);

			transform.SetWorldPosition(physicsBody.GetPosition());

			DebugRenderer::DrawRectangle(transform.GetWorldPosition(), 16.f, 16.f);
		}
		world->ClearForces();
	}
}
