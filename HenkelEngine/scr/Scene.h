#pragma once

#include <vector>
#include "Entity\Entity.h"
#include <memory>
#include "Camera.h"
#include <string>
#include "UI\Panel.h"

class Window;

class Scene
{
public :
	Scene(Window* window);
	~Scene();

	void Update();
	void Render();

	Entity* CreateEntity(std::string name);
	void RemoveEntity(Entity* entity);
	const std::vector<std::unique_ptr<Entity>>& GetEntities() const { return m_entities; }

	Camera* GetCamera() const { return m_camera.get(); }
	Window* GetWindow() const { return m_window; }

private:

	void CreateDockingEnviroment();
	void InitDockingEnviroment(ImGuiID dockspaceID, ImGuiDockNodeFlags dockspace_flags);

	void CreatePanels(ImGuiID dockspaceID);

	std::vector<std::unique_ptr<Entity>> m_entities;
	std::vector<std::unique_ptr<Panel>> m_panels;
	std::unique_ptr<Camera> m_camera;
	Window* m_window;
	bool m_dockingEnviromentInited;
};

