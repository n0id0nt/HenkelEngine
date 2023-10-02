#pragma once

#include <vector>
#include "Entity\Entity.h"
#include <memory>
#include "Camera.h"
#include <string>

class Window;

class Scene
{
public :
	Scene(Window* window, const std::string& fileDir, const std::string& levelFile);
	~Scene();

	void LoadScene(const std::string& fileDir, const std::string& levelFile);

	void Update();
	void Render();

	Entity* CreateEntity(std::string name);
	void RemoveEntity(Entity* entity);
	const std::vector<std::unique_ptr<Entity>>& GetEntities() const { return m_entities; }

	Camera* GetCamera() const { return m_camera.get(); }
	Window* GetWindow() const { return m_window; }

private:
	std::vector<std::unique_ptr<Entity>> m_entities;
	std::unique_ptr<Camera> m_camera;
	Window* m_window;
	bool m_dockingEnviromentInited;

	std::string m_name;
};

