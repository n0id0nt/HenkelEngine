#include "Scene.h"
#include "ECS\Component\RenderComponents\SpriteComponent.h"
#include "ECS\Component\RenderComponents\TileMapComponent.h"
#include "ECS\Component\PlayerMovementComponent.h"
#include "Engine.h"
#include "pugixml.hpp"
#include <fstream>
#include <iostream>
#include "base64.h"
#include "HelperFunctions.h"
#include "zlib.h"
#include "Resourse\TileSheet.h"
#include "ECS\Component\ColliderComponent\RectangleColliderComponent.h"
#include "opengl\DebugRenderer.h"
#include "ECS\Component\PhysicsBodyComponents\PhysicsBodyComponent.h"
#include "ECS\Component\PhysicsBodyComponents\StaticBodyComponent.h"
#include "ECS\Component\PhysicsBodyComponents\TileMapCollisionBodyComponent.h"
#include <imgui.h>

//Entity* cube, *cube2;
float x = 0.f, y = 0.f, z = 2.f;
const float timeStep = 1.0f / 60.0f;
const int velocityIterations = 6;
const int positionIterations = 2;


Scene::Scene(Engine* engine, const std::string& fileDir, const std::string& levelFile) 
	: m_entities(), m_engine(engine), m_dockingEnviromentInited(false), m_name("Scene")
{
	m_camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 0.0f));
	m_world = std::make_unique<PhysicsWorld>(glm::vec2{ 0.f,0.f }, timeStep, velocityIterations, positionIterations);
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

			timemap->GetTransform()->SetScale({ tileSheet.GetTileWidth(), tileSheet.GetTileHeight(), 1.f});
			timemap->AddComponent(new TileMapComponent(timemap, m_engine, width, height, levelArray, tileSheet));
			timemap->AddComponent(new TileMapCollisionBodyComponent(timemap, m_world.get()));
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
				gameObject->GetTransform()->SetScale({ object.attribute("width").as_float(), object.attribute("height").as_float(), 1.f });
				gameObject->GetTransform()->SetParent(objectGroup->GetTransform());
				gameObject->GetTransform()->SetPosition({ object.attribute("x").as_float(), object.attribute("y").as_float() - object.attribute("height").as_float(), 0.f });
				gameObject->AddComponent(new RectangleColliderComponent(gameObject, object.attribute("width").as_float(), object.attribute("height").as_float()));
				gameObject->AddComponent(new SpriteComponent(gameObject, m_engine, tileSheet, object.attribute("gid").as_uint() - 1));
				if (name == "Player")
				{
					gameObject->AddComponent(new PhysicsBodyComponent(gameObject, m_world.get()));
					gameObject->AddComponent(new PlayerMovementComponent(gameObject, m_engine));
				}
				else
				{
					gameObject->AddComponent(new StaticBodyComponent(gameObject, m_world.get()));
				}
			}
		}
	}
}

void Scene::Update(float deltaTime)
{
	m_world->Step();

	for (auto& entity : m_entities)
	{
		if (entity->HasComponent<PhysicsBodyComponent>())
		{
			entity->GetComponent<PhysicsBodyComponent>()->UpdatePos();
		}
	}

	for (auto& entity : m_entities)
	{
		entity->Update(deltaTime);
	}
	m_camera->SetPosition({ x, y, 0.f });
	m_camera->SetZoom(z);



	m_world->ClearForces();
}

void Scene::Render()
{
	ImGui::Begin("Camera Controls");
	ImGui::SliderFloat("X", &x, -300.0f, 300.0f);
	ImGui::SliderFloat("Y", &y, -300.0f, 300.0f);
	ImGui::SliderFloat("Zoom", &z, 0.5f, 20.0f);

	ImGui::End();

	DebugRenderer::DrawRectangle({ 29.f * 8.f, 19.f * 8.f, 0.f }, 30.f * 16.f, 20.f * 16.f, {0.5f,0.5f,0.5f});

	glm::mat4 projection = m_camera->CalculateProjection(m_engine->GetWindow()->GetWidth(), m_engine->GetWindow()->GetHeight());
	glm::mat4 view = m_camera->GetViewMatrix();
	DebugRenderer::Render(projection * view);

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

