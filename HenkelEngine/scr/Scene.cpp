#include "Scene.h"
#include "Component\SpriteComponent.h"
#include "Component\TileMapComponent.h"
#include "Window.h"
#include "pugixml.hpp"
#include <fstream>
#include <iostream>
#include "base64.h"
#include "HelperFunctions.h"
#include "zlib.h"
#include "Resourse\TileSheet.h"

//Entity* cube, *cube2;
float x = -0.95f, y = 1.f, z = 32.f;

Scene::Scene(Window* window, const std::string& fileDir, const std::string& levelFile) : m_entities(), m_camera(new Camera(glm::vec3(0.0f, 0.0f, 0.0f))), m_window(window), m_dockingEnviromentInited(false), m_name("Scene")
{
	// create the rubiks cube and respective components
	//cube = CreateEntity("Cube");
	//cube->AddComponent(new SpriteComponent(cube));
	//cube->GetTransform()->SetScale({ 3.f, 3.f, 1.f });
	//cube->GetTransform()->SetRotation({ 30.5f, 0.f, 0.f });
	//
	//cube2 = CreateEntity("Cube2");
	//cube2->AddComponent(new SpriteComponent(cube2));
	//cube2->GetTransform()->SetScale({ 10.f, 10.f, 1.f });
	//cube2->GetTransform()->SetPosition({ -50.5f, -50.5f, 1.f });

	m_camera->SetOrthographic(true);

	LoadScene(fileDir, levelFile);
}

Scene::~Scene()
{
	
}

void Scene::LoadScene(const std::string& fileDir, const std::string& levelFile)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file((fileDir + levelFile).c_str());
	ASSERT(result);

	// level information to know how to read the file
	m_name = doc.child("map").attribute("class").as_string();
	ASSERT(std::string(doc.child("map").attribute("orientation").as_string()) == "orthogonal");
	ASSERT(std::string(doc.child("map").attribute("renderorder").as_string()) == "right-down");

	TileSheet tileSheet(fileDir, doc.child("map").child("tileset").attribute("source").as_string());

	for (auto& layer : doc.child("map").children("layer"))
	{
		// create tilemap component
		Entity* timemap = CreateEntity(layer.attribute("name").as_string());
		int width = layer.attribute("width").as_int();
		int height = layer.attribute("height").as_int();

		// retrieve level data
		ASSERT(std::string(layer.child("data").attribute("encoding").as_string()) == "base64");
		std::string base64Data = layer.child("data").text().as_string();
		std::string data = base64_decode(HenkelEngine::trim(base64Data));
		uLongf numGids = width * height *sizeof(int);
		std::vector<unsigned> levelArray(numGids);
		uncompress((Bytef*)levelArray.data(), &numGids, (const Bytef*)data.c_str(), data.size());
		levelArray.erase(levelArray.begin() + width * height, levelArray.end());

		timemap->AddComponent(new TileMapComponent(timemap, width, height, levelArray, tileSheet));
		//timemap->GetTransform()->SetScale({ 32.f, 32.f, 1.f });
		
	}
}

void Scene::Update()
{
	for (auto& entity : m_entities)
	{
		entity->Update();
		entity->GetTransform()->SetPosition({ x, y, 0.f });
		entity->GetTransform()->SetScale({ z, z, 1.f });
	}
	//cube->GetTransform()->SetPosition({ x, y, z });
	//cube2->GetTransform()->SetPosition({ x, y, 0.f });
}

void Scene::Render()
{
	ImGui::Begin("My First Tool");
	ImGui::SliderFloat("x", &x, -3.0f, 3.f);
	ImGui::SliderFloat("y", &y, -3.0f, 3.0f);
	ImGui::SliderFloat("Scale", &z, 1.0f, 100.0f);

	ImGui::End();

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

