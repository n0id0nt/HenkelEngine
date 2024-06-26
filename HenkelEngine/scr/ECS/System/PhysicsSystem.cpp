#include "PhysicsSystem.h"
#include "ECS/Component/PhysicsBodyComponents/PhysicsBodyComponent.h"
#include "ECS/Component/PhysicsBodyComponents/StaticBodyComponent.h"
#include "ECS/Component/PhysicsBodyComponents/TileMapCollisionBodyComponent.h"
#include "ECS/Component/TransformComponent.h"
#include "opengl\DebugRenderer.h"
#include <iostream>

void PhysicsSystem::Update(Registry* registry, PhysicsWorld* world)
{
	world->Step();
	world->ClearForces();

#ifdef _DEBUG
	//Draw debug shapes
	{
		auto view = registry->GetEntitiesWithComponents<StaticBodyComponent, TransformComponent>();
		for (auto& entity : view)
		{
			auto* staticBody = registry->GetComponent<StaticBodyComponent>(entity);
			//auto staticBody = view.get<StaticBodyComponent>(entity);
			glm::vec2 collisionShape = staticBody->GetCollisionShape();

			DebugRenderer::DrawRectangle(glm::vec3{staticBody->GetPosition(), 0.f}, collisionShape.x, collisionShape.y, { 0.f, 0.5f, 0.8f });
		}
	}
	{
		auto view = registry->GetEntitiesWithComponents<TileMapCollisionBodyComponent, TransformComponent>();
		for (auto& entity : view)
		{
			auto* transform = registry->GetComponent<TransformComponent>(entity);
			auto* tileMap = registry->GetComponent<TileMapCollisionBodyComponent>(entity);
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
							DebugRenderer::DrawLine(glm::vec3{prevVertex.x * world->GetPixelsPerMeter(), prevVertex.y * world->GetPixelsPerMeter(), 0.f}, glm::vec3{vertex.x * world->GetPixelsPerMeter(), vertex.y * world->GetPixelsPerMeter(), 0.f}, fixture->IsSensor() ? glm::vec3{ 0.2f, 0.8f, 0.8f } : glm::vec3{ 0.8f, 0.5f, 0.f });
							prevVertex = vertex;
						}
					}
				}
			}
		}
	}
#endif //_DEBUG
	{
		auto view = registry->GetEntitiesWithComponents<PhysicsBodyComponent, TransformComponent>();
		for (auto& entity : view)
		{
			//std::cout << (int)entity << std::endl;
			auto* transform = registry->GetComponent<TransformComponent>(entity);
			auto* physicsBody = registry->GetComponent<PhysicsBodyComponent>(entity);
			
			transform->SetWorldPosition(physicsBody->GetPosition());
#ifdef _DEBUG
			glm::vec2 collisionShape = physicsBody->GetCollisionShape();
			DebugRenderer::DrawRectangle(glm::vec3{physicsBody->GetPosition(), 0.f}, collisionShape.x, collisionShape.y);
#endif // _DEBUG
		}
	}
}
