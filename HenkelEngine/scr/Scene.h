#pragma once

#include <vector>
#include <memory>
#include "Camera.h"
#include <string>
#include "box2d\box2d.h"
#include "Physics\PhysicsWorld.h"
#include "ECS\Registry.h"
#include "ECS\System\AnimationSystem.h"
#include "ECS\System\PhysicsSystem.h"
#include "ECS\System\RenderSystem.h"
#include "ECS\System\ScriptSystem.h"
#include "ECS\System\UISystem.h"
#include "pugixml.hpp"
#include "Resourse\TileSheet.h"
#include "sol\sol.hpp"
#include <ECS\System\CameraSystem.h>

class Scene : public LUABindable
{
public :
	Scene(const std::string& fileDir, const std::string& levelFile);
	~Scene();

	void LoadScene(const std::string& fileDir, const std::string& levelFile);
	Entity* LoadTemplate(const std::string& fileDir, const std::string& levelFile);
	Entity* CreateObject(const pugi::xml_node& object, const std::string& fileDir, const TileSheet& tileSheet);
	Entity* CreateTemplatedObject(const std::string& levelFile);

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

	Entity* LoadUILayout(const std::string& fileDir, const std::string& levelFile);
	std::unique_ptr<UIArea> CreateUIElement(const pugi::xml_node& object, const std::string& fileDir);

	Entity* CreateEntity(const std::string& name);

	void Update();
	void Render();

	Camera* GetCamera() const { return m_camera.get(); }

	PhysicsWorld* GetPhysicsWorld() const { return m_world.get(); }

	sol::state& GetLuaState();

	void LUABind(sol::state& lua);

private:
	std::unique_ptr<Camera> m_camera;
	std::vector<std::unique_ptr<Entity>> m_entities;

	std::string m_name;

	std::unique_ptr<PhysicsWorld> m_world;

	Registry m_registry;

	AnimationSystem m_animationSystem;
	PhysicsSystem m_physicsSystem;
	RenderSystem m_renderSystem;
	ScriptSystem m_scriptSystem;
	CameraSystem m_cameraSystem;
	UISystem m_uiSystem;

	//TODO should probably remove this, so I can mave multile camera ie, one for ui
	int m_activeCameraId = 0;
};

