#pragma once

#include <vector>
#include <memory>
#include "Camera.h"
#include <string>
#include "box2d.h"
#include "Physics\PhysicsWorld.h"
#include "entt.hpp"
#include "ECS\System\AnimationSystem.h"
#include "ECS\System\PhysicsSystem.h"
#include "ECS\System\RenderSystem.h"
#include "ECS\System\ScriptSystem.h"
#include "pugixml.hpp"
#include "Resourse\TileSheet.h"


class Engine;

class Scene
{
public :
	Scene(Engine* engine, const std::string& fileDir, const std::string& levelFile);
	~Scene();

	void LoadScene(const std::string& fileDir, const std::string& levelFile);
	void LoadTemplate(const std::string& fileDir, const std::string& levelFile);
	void CreateObject(const pugi::xml_node& object, const std::string& fileDir, const TileSheet& tileSheet);

	void Update(float deltaTime);
	void Render();

	Camera* GetCamera() const { return m_camera.get(); }

	PhysicsWorld* GetPhysicsWorld() const { return m_world.get(); }

private:
	Engine* m_engine;

	std::unique_ptr<Camera> m_camera;
	bool m_dockingEnviromentInited;

	std::string m_name;

	std::unique_ptr<PhysicsWorld> m_world;

	entt::registry m_registry;

	AnimationSystem m_animationSystem;
	PhysicsSystem m_physicsSystem;
	RenderSystem m_renderSystem;
	ScriptSystem m_scriptSystem;
};

