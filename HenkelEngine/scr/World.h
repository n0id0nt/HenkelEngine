#pragma once

#include <vector>
#include <string>
#include "Camera.h"
#include "LUABindable.h"
#include "ECS\Registry.h"
#include "sol\sol.hpp"
#include "Physics\PhysicsWorld.h"

class World : public LUABindable
{
public:
	World();
	~World();

	void Update();
	void Render();

	Camera* GetCamera() const;
	PhysicsWorld* GetPhysicsWorld() const;

	Entity* CreateTemplatedObject(const std::string& levelFile);
	Entity* CreateEntity(const std::string& name);
	void DeleteEntity(Entity* entity);
	Entity* GetEntityFromEntityID(EntityId entityId);

	template <typename ComponentType>
	std::vector<Entity*> GetEntitiesWithComponent()
	{
		std::vector<EntityId> entityIds = m_registry.GetEntitiesWithComponent<ComponentType>();
		std::vector<Entity*> entities;
		// TODO there has to be a better way that this 2d loop, like the order of this somewhat corresponst to each other so there has to be some correlation I can use
		for (EntityId entityId : entityIds)
		{
			for (auto& entity : m_entities)
			{
				if (entity->GetEntityId() == entityId)
				{
					entities.push_back(entity.get());
				}
			}
		}
		return entities;
	}
	template <typename... ComponentTypes>
	std::vector<Entity*> GetEntitiesWithComponents()
	{
		std::vector<EntityId> entityIds = m_registry.GetEntitiesWithComponents<ComponentTypes...>();
		std::vector<Entity*> entities;
		// TODO there has to be a better way that this 2d loop, like the order of this somewhat corresponst to each other so there has to be some correlation I can use
		for (EntityId entityId : entityIds)
		{
			for (auto& entity : m_entities)
			{
				if (entity->GetEntityId() == entityId)
				{
					entities.push_back(entity.get());
				}
			}
		}
		return entities;
	}
	template <typename ComponentType>
	std::vector<ComponentType*> GetAllComponents()
	{
		return m_registry.GetAllComponents<ComponentType>();
	}

	void LUABind(sol::state& lua);

private:

	std::unique_ptr<Camera> m_camera;
	std::vector<std::unique_ptr<Entity>> m_entities;

	std::unique_ptr<PhysicsWorld> m_physicsWorld;

	Registry m_registry;

	// TODO should probably remove this, so I can mave multile camera ie, one for ui
	int m_activeCameraId = 0;
};

