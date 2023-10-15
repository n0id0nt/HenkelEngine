#include "Scene.h"
#include "Component\SpriteComponent.h"
#include "Component\TileMapComponent.h"
#include "Component\PlayerMovementComponent.h"
#include "Window.h"
#include "pugixml.hpp"
#include <fstream>
#include <iostream>
#include "base64.h"
#include "HelperFunctions.h"
#include "zlib.h"
#include "Resourse\TileSheet.h"

//Entity* cube, *cube2;
float x = 0.f, y = 0.f, z = 2.f;
const float timeStep = 1.0f / 60.0f;
const int32 velocityIterations = 6;
const int32 positionIterations = 2;


Scene::Scene(Window* window, const std::string& fileDir, const std::string& levelFile) 
	: m_entities(), m_camera(new Camera(glm::vec3(0.0f, 0.0f, 0.0f))), m_window(window), m_dockingEnviromentInited(false), m_name("Scene"), m_world({0.f,0.f})
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

	for (auto& layer : doc.child("map").children())
	{
		std::string name(layer.name());
		if (name == "layer")
		{
			// create tilemap component
			Entity* timemap = CreateEntity(layer.attribute("name").as_string());
			int width = layer.attribute("width").as_int();
			int height = layer.attribute("height").as_int();

			// retrieve level data
			ASSERT(std::string(layer.child("data").attribute("encoding").as_string()) == "base64");
			std::string base64Data = layer.child("data").text().as_string();
			std::string data = base64_decode(HenkelEngine::trim(base64Data));
			uLongf numGids = width * height * sizeof(int);
			std::vector<unsigned> levelArray(numGids);
			uncompress((Bytef*)levelArray.data(), &numGids, (const Bytef*)data.c_str(), data.size());
			levelArray.erase(levelArray.begin() + width * height, levelArray.end());

			timemap->AddComponent(new TileMapComponent(timemap, width, height, levelArray, tileSheet));
			timemap->GetTransform()->SetScale({ tileSheet.GetTileWidth(), tileSheet.GetTileHeight(), 1.f});
		}
		else if (name == "objectgroup")
		{
			// create tilemap component
			Entity* objectGroup = CreateEntity(layer.attribute("name").as_string());
			objectGroup->GetTransform()->SetPosition({ layer.attribute("offsetx").as_float(), layer.attribute("offsety").as_float(), 0.f });
			for (auto& object : layer.children("object"))
			{
				Entity* gameObject = CreateEntity(object.attribute("name").as_string());
				std::string name = object.attribute("name").as_string();
				if (name == "Player")
				{
					gameObject->AddComponent(new PlayerMovementComponent(gameObject));
				}
				gameObject->AddComponent(new SpriteComponent(gameObject, tileSheet, object.attribute("gid").as_uint() - 1));
				gameObject->GetTransform()->SetParent(objectGroup->GetTransform());
				gameObject->GetTransform()->SetPosition({ object.attribute("x").as_float(), object.attribute("y").as_float() - object.attribute("height").as_float(), 0.f });
				gameObject->GetTransform()->SetScale({ object.attribute("width").as_float(), object.attribute("height").as_float(), 1.f });
			}
		}
	}
}

void Scene::Update(float deltaTime)
{
	m_world.Step(timeStep, velocityIterations, positionIterations);
	for (auto& entity : m_entities)
	{
		entity->Update(deltaTime);
	}
	m_camera->SetPosition({ x, y, 0.f });
	m_camera->SetZoom(z);
}

void Scene::Render()
{
	ImGui::Begin("Camera Controls");
	ImGui::SliderFloat("X", &x, -300.0f, 300.0f);
	ImGui::SliderFloat("Y", &y, -300.0f, 300.0f);
	ImGui::SliderFloat("Zoom", &z, 0.5f, 20.0f);

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

