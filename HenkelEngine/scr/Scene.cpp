#include "Scene.h"
#include "ECS\Component\RenderComponents\SpriteComponent.h"
#include "ECS\Component\RenderComponents\TileMapComponent.h"
#include "ECS\Component\RenderComponents\RenderComponent.h"
#include "Engine.h"
#include <fstream>
#include <iostream>
#include "base64.h"
#include "HelperFunctions.h"
#include "zlib.h"
#include "opengl\DebugRenderer.h"
#include "ECS\Component\PhysicsBodyComponents\PhysicsBodyComponent.h"
#include "ECS\Component\PhysicsBodyComponents\StaticBodyComponent.h"
#include "ECS\Component\PhysicsBodyComponents\TileMapCollisionBodyComponent.h"
#include "ECS\Component\ScriptComponent.h"
#include <imgui.h>
#include <ECS\Component\TransformComponent.h>

//Entity* cube, *cube2;
float x = 0.f, y = 0.f, z = 2.f;
const float timeStep = 1.0f / 60.0f;
const int velocityIterations = 6;
const int positionIterations = 2;


Scene::Scene(Engine* engine, const std::string& fileDir, const std::string& levelFile) 
	: m_engine(engine), m_name("Scene"), 
	m_registry(), m_animationSystem(&m_registry), m_physicsSystem(&m_registry, engine), m_renderSystem(&m_registry, engine), m_scriptSystem(&m_registry), m_entities()
{
	m_camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 0.0f));

	m_scriptSystem.BindToLua(*m_engine->GetInput());
	m_scriptSystem.BindToLua(*this);

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

	// Custom Properties
	float pixelsPerMeter = 1.f;
	for (auto& property : doc.child("map").child("properties").children())
	{
		std::string name(property.attribute("name").as_string());
		if (name == "Pixels Per Meter")
		{
			pixelsPerMeter = property.attribute("value").as_float();
		}
	}

	m_world = std::make_unique<PhysicsWorld>(glm::vec2{ 0.f, 0.f }, timeStep, velocityIterations, positionIterations, pixelsPerMeter);


	TileSheet tileSheet(m_engine->GetProjectDirectory(), fileDir + doc.child("map").child("tileset").attribute("source").as_string());

	for (auto& layer : doc.child("map").children())
	{
		std::string name(layer.name());
		if (name == "layer")
		{
			// create tilemap component
			Entity* tilemapEntity = CreateEntity(layer.attribute("name").as_string());
			unsigned int width = layer.attribute("width").as_uint();
			unsigned int height = layer.attribute("height").as_uint();

			// retrieve level data
			ASSERT(std::string(layer.child("data").attribute("encoding").as_string()) == "base64");
			std::string base64Data = layer.child("data").text().as_string();
			std::string data = base64_decode(HenkelEngine::trim(base64Data));
			uLongf numGids = width * height * sizeof(int);
			std::vector<unsigned> levelArray(numGids);
			uncompress((Bytef*)levelArray.data(), &numGids, (const Bytef*)data.c_str(), data.size());
			levelArray.erase(levelArray.begin() + width * height, levelArray.end());

			tilemapEntity->CreateComponent<MaterialComponent>(tileSheet.GetTileSetImagePath(), "res/shaders/sprite.vert", "res/shaders/sprite.frag", m_engine);
			tilemapEntity->CreateComponent<RenderComponent>(width * height);
			auto* tilemap = tilemapEntity->CreateComponent<TileMapComponent>(width, height, levelArray, tileSheet);
			auto* transform = tilemapEntity->CreateComponent<TransformComponent>(glm::vec3(), glm::vec3(), glm::vec3{ tileSheet.GetTileWidth(), tileSheet.GetTileHeight(), 1.f });
			
			b2BodyDef bodyDef;
			glm::vec2 position = transform->GetWorldPosition();
			bodyDef.position = b2Vec2(position.x, position.y);
			bodyDef.fixedRotation = true;
			//bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(gameObjectEntity);
			
			b2FixtureDef fixtureDef;
			b2PolygonShape shape;
			shape.SetAsBox(tileSheet.GetTileWidth() / (2.f * m_world->GetPixelsPerMeter()), tileSheet.GetTileHeight() / (2.f * m_world->GetPixelsPerMeter()));
			fixtureDef.shape = &shape;
			fixtureDef.friction = 0.f;
			fixtureDef.density = 1.f;
			tilemapEntity->CreateComponent<TileMapCollisionBodyComponent>(m_world.get(), fixtureDef, bodyDef, *tilemap);
		}
		else if (name == "objectgroup")
		{
			// create tilemap component
			Entity* tilemapEntity = CreateEntity(layer.attribute("name").as_string());
			auto* objectGroupTransform = tilemapEntity->CreateComponent<TransformComponent>(glm::vec3{ layer.attribute("offsetx").as_float(), layer.attribute("offsety").as_float(), 0.f }, glm::vec3(), glm::vec3{1.f,1.f,1.f});

			for (auto& object : layer.children("object"))
			{
				auto* gameObjectEntity = CreateObject(object, fileDir, tileSheet);
				auto transform = gameObjectEntity->GetComponent<TransformComponent>();
				if (transform)
				{
					transform->SetParent(objectGroupTransform);
				}
				auto physicsBody = gameObjectEntity->GetComponent<PhysicsBodyComponent>();
				auto staticBody = gameObjectEntity->GetComponent<StaticBodyComponent>();
				if (physicsBody)
				{
					physicsBody->SetPosition(transform->GetWorldPosition());
				}
				if (staticBody)
				{
					staticBody->SetPosition(transform->GetWorldPosition());
				}
			}
		}
	}
}

Entity* Scene::LoadTemplate(const std::string& fileDir, const std::string& levelFile)
{
	// Get Working Dir of new file
	std::string file = fileDir + levelFile;
	std::string workingDir = HenkelEngine::GetFileDir(file);
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(file.c_str());
	ASSERT(result);

	TileSheet tileSheet(m_engine->GetProjectDirectory(), workingDir + doc.child("template").child("tileset").attribute("source").as_string());

	return CreateObject(doc.child("template").child("object"), workingDir, tileSheet);
}

Entity* Scene::CreateObject(const pugi::xml_node& object, const std::string& fileDir, const TileSheet& tileSheet)
{
	Entity* gameObjectEntity = nullptr;

	glm::vec3 objectPosition{ object.attribute("x") ? object.attribute("x").as_float() : 0.f, object.attribute("y") ? object.attribute("y").as_float() - tileSheet.GetTileHeight() : 0.f, 0.f };

	if (object.attribute("template"))
	{
		gameObjectEntity = LoadTemplate(fileDir, object.attribute("template").as_string());
		auto tranformComponent = gameObjectEntity->GetComponent<TransformComponent>();
		auto physicsBody = gameObjectEntity->GetComponent<PhysicsBodyComponent>();
		auto staticBody = gameObjectEntity->GetComponent<StaticBodyComponent>();
		if (tranformComponent)
		{
			tranformComponent->SetPosition(objectPosition);
		}
		if (physicsBody)
		{
			physicsBody->SetPosition(objectPosition);
		}
		if (staticBody)
		{
			staticBody->SetPosition(objectPosition);
		}
	}
	else
	{
		std::string objectName = object.attribute("name").as_string();
		gameObjectEntity = CreateEntity(objectName);

		gameObjectEntity->CreateComponent<MaterialComponent>(tileSheet.GetTileSetImagePath(), "res/shaders/sprite.vert", "res/shaders/sprite.frag", m_engine);
		gameObjectEntity->CreateComponent<RenderComponent>(1u);
		gameObjectEntity->CreateComponent<SpriteComponent>(tileSheet, object.attribute("gid").as_uint() - 1);
		auto* transform = gameObjectEntity->CreateComponent<TransformComponent>(objectPosition, glm::vec3(), glm::vec3{ tileSheet.GetTileWidth(), tileSheet.GetTileHeight(), 1.f });
		

		b2BodyDef bodyDef;
		glm::vec2 position = transform->GetWorldPosition();
		bodyDef.position = b2Vec2(position.x, position.y);
		bodyDef.fixedRotation = true;
		//bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(gameObjectEntity);

		b2FixtureDef fixtureDef;
		b2PolygonShape shape;
		shape.SetAsBox(object.attribute("width").as_float() / (2.f * m_world->GetPixelsPerMeter()), object.attribute("height").as_float() / (2.f * m_world->GetPixelsPerMeter()));
		fixtureDef.shape = &shape;
		fixtureDef.friction = 0.f;
		fixtureDef.density = 1.f;

		for (auto& property : object.child("properties").children())
		{
			std::string propertyName(property.attribute("name").as_string());
			if (propertyName == "Script")
			{
				std::string script = property.attribute("value").as_string();
				gameObjectEntity->CreateComponent<ScriptComponent>(fileDir + script);
			}
		}

		if (objectName == "Player")
		{
			gameObjectEntity->CreateComponent<PhysicsBodyComponent>(m_world.get(), fixtureDef, bodyDef);
		}
		else
		{
			gameObjectEntity->CreateComponent<StaticBodyComponent>(m_world.get(), fixtureDef, bodyDef);
		}
	}

	return gameObjectEntity;
}

Entity* Scene::CreateTemplatedObject(const std::string& levelFile)
{
	return LoadTemplate(m_engine->GetProjectDirectory(), levelFile);
}

Entity* Scene::CreateEntity(const std::string& name)
{
	m_entities.push_back(Entity(name, &m_registry));
	return &m_entities.back();
}

void Scene::Update(float deltaTime)
{
	m_physicsSystem.Update(m_world.get());

	m_scriptSystem.Update(deltaTime);

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

	DebugRenderer::DrawRectangle({ 29.f * 8.f, 19.f * 8.f, 0.f }, 30.f * 16.f, 20.f * 16.f, {0.5f,0.5f,0.5f});

	glm::mat4 projection = m_camera->CalculateProjection(m_engine->GetWindow()->GetWidth(), m_engine->GetWindow()->GetHeight());
	glm::mat4 view = m_camera->GetViewMatrix();
	DebugRenderer::Render(projection * view);

	m_renderSystem.Update();
}

void Scene::LUABind(sol::state& lua)
{
	lua.new_usertype<Scene>("Scene",
		"createTemplatedObject", &Scene::CreateTemplatedObject
	);
	lua.set("Scene", this);
}

