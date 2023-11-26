#include "PhysicsSystem.h"
#include "ECS/Component/PhysicsBodyComponents/PhysicsBodyComponent.h"
#include "ECS/Component/PhysicsBodyComponents/StaticBodyComponent.h"
#include "ECS/Component/PhysicsBodyComponents/TileMapCollisionBodyComponent.h"
#include "ECS/Component/TransformComponent.h"
#include "opengl\DebugRenderer.h"
#include <iostream>

PhysicsSystem::PhysicsSystem(Registry* registry) 
	: m_registry(registry)
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
			glm::vec2 collisionShape = staticBody->GetCollisionShape();

			DebugRenderer::DrawRectangle(glm::vec3{staticBody->GetPosition(), 0.f}, collisionShape.x, collisionShape.y, { 0.f, 0.5f, 0.8f });
		}
	}
	{
		auto view = m_registry->GetEntitiesWithComponents<TileMapCollisionBodyComponent, TransformComponent>();
		for (auto& entity : view)
		{
			auto* transform = m_registry->GetComponent<TransformComponent>(entity);
			auto* tileMap = m_registry->GetComponent<TileMapCollisionBodyComponent>(entity);
			for (b2Fixture* fixture = tileMap->GetBody()->GetFixtureList(); fixture; fixture = fixture->GetNext())
			{
				b2Shape* shape = fixture->GetShape();

				if (shape->GetType() == b2Shape::e_chain) 
				{
					b2ChainShape* chainShape = dynamic_cast<b2ChainShape*>(shape);
					if (chainShape)
					{
						b2Vec2 prevVertex = tileMap->GetBody()->GetWorldPoint(chainShape->m_vertices[0]);
						for (int i = 1; i < chainShape->m_count; ++i)
						{
							b2Vec2 vertex = tileMap->GetBody()->GetWorldPoint(chainShape->m_vertices[i]);
							DebugRenderer::DrawLine(glm::vec3{prevVertex.x * world->GetPixelsPerMeter(), prevVertex.y * world->GetPixelsPerMeter(), 0.f}, glm::vec3{vertex.x * world->GetPixelsPerMeter(), vertex.y * world->GetPixelsPerMeter(), 0.f}, { 0.8f, 0.5f, 0.f });
							prevVertex = vertex;
						}
					}
				}
			}
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
			glm::vec2 collisionShape = physicsBody->GetCollisionShape();
			DebugRenderer::DrawRectangle(glm::vec3{physicsBody->GetPosition(), 0.f}, collisionShape.x, collisionShape.y);
		}
	}
}
