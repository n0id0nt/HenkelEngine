#include "Scene.h"
#include "Component\SpriteComponent.h"
#include "Window.h"

Scene::Scene(Window* window) : m_entities(), m_camera(new Camera(glm::vec3(0.0f, 0.0f, 0.0f))), m_window(window), m_dockingEnviromentInited(false)
{
	// create the rubiks cube and respective components
	Entity* cube = CreateEntity("Cube");
	cube->AddComponent(new SpriteComponent(cube));
	//cube->GetTransform()->SetScale({ 100.f, 100.f, 1.f });
	
	Entity* cube2 = CreateEntity("Cube2");
	cube2->AddComponent(new SpriteComponent(cube2));
	cube2->GetTransform()->SetPosition({ 0.5f, 0.5f, 1.f });
	//cube2->GetTransform()->SetRotation({ 30.5f, 50.f, -80.f });
	cube2->GetTransform()->SetScale({ 200.f, 80.f, 1.f });

	m_camera->SetOrthographic(true);
}

Scene::~Scene()
{
	
}

void Scene::Update()
{
	for (auto& entity : m_entities)
	{
		entity->Update();
	}
}

void Scene::Render()
{
	for (auto& entity : m_entities)
	{
		if (entity->HasComponent<RenderComponent>())
		{
			entity->GetComponent<RenderComponent>()->Render();
		}
	}
}

Entity* Scene::CreateEntity(std::string name)
{
	m_entities.push_back(std::make_unique<Entity>(this, name));
	return m_entities.back().get();
}

void Scene::RemoveEntity(Entity* entity)
{
}

