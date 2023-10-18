#pragma once

#include <vector>
#include "Entity\Entity.h"
#include <memory>
#include "Camera.h"
#include <string>
#include "box2d.h"

class Window;

class Scene
{
public :
	Scene(Window* window, const std::string& fileDir, const std::string& levelFile);
	~Scene();

	void LoadScene(const std::string& fileDir, const std::string& levelFile);

	void Update(float deltaTime);
	void Render();

	Entity* CreateEntity(std::string name);
	void RemoveEntity(Entity* entity);
	const std::vector<std::unique_ptr<Entity>>& GetEntities() const { return m_entities; }

	Camera* GetCamera() const { return m_camera.get(); }
	Window* GetWindow() const { return m_window; }

	b2World* GetWorld() const { return m_world.get(); }

private:
	std::vector<std::unique_ptr<Entity>> m_entities;
	std::unique_ptr<Camera> m_camera;
	Window* m_window;
	bool m_dockingEnviromentInited;

	std::string m_name;

	std::unique_ptr<b2World> m_world;
};

