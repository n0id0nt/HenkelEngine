#pragma once

#include <vector>
#include "ECS\Entity\Entity.h"
#include <memory>
#include "Camera.h"
#include <string>
#include "box2d.h"
#include "Physics\PhysicsWorld.h"

class Engine;

class Scene
{
public :
	Scene(Engine* engine, const std::string& fileDir, const std::string& levelFile);
	~Scene();

	void LoadScene(const std::string& fileDir, const std::string& levelFile);

	void Update(float deltaTime);
	void Render();

	Entity* CreateEntity(std::string name);
	void RemoveEntity(Entity* entity);
	const std::vector<std::unique_ptr<Entity>>& GetEntities() const { return m_entities; }

	Camera* GetCamera() const { return m_camera.get(); }

	PhysicsWorld* GetPhysicsWorld() const { return m_world.get(); }

private:
	Engine* m_engine;

	std::vector<std::unique_ptr<Entity>> m_entities;
	std::unique_ptr<Camera> m_camera;
	bool m_dockingEnviromentInited;

	std::string m_name;

	std::unique_ptr<PhysicsWorld> m_world;
};

