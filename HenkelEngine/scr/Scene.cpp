#include "Scene.h"
#include "ECS\Component\RenderComponents\SpriteComponent.h"
#include "ECS\Component\RenderComponents\TileMapComponent.h"
#include "ECS\Component\RenderComponents\RenderComponent.h"
#include "Engine.h"
#include <fstream>
#include <iostream>
#include <regex>
#include "base64.h"
#include "HelperFunctions.h"
#include "zlib.h"
#include "opengl\DebugRenderer.h"
#include "ECS\Component\PhysicsBodyComponents\PhysicsBodyComponent.h"
#include "ECS\Component\PhysicsBodyComponents\StaticBodyComponent.h"
#include "ECS\Component\PhysicsBodyComponents\TileMapCollisionBodyComponent.h"
#include "ECS\Component\ScriptComponent.h"
#include "ECS\Component\SpriteAnimationComponent.h"
#include <imgui.h>
#include <ECS\Component\TransformComponent.h>
#include "DebugGUIPanels\GUIPanels.h"
#include <ECS\Component\CameraComponent.h>

const float timeStep = 1.0f / FPS;
const int velocityIterations = 6;
const int positionIterations = 4;


Scene::Scene(const std::string& fileDir, const std::string& levelFile) 
	: m_name("Scene"), m_registry(), m_animationSystem(&m_registry), m_physicsSystem(&m_registry), m_renderSystem(&m_registry), m_scriptSystem(&m_registry), m_cameraSystem(&m_registry), m_entities()
{
	m_camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 0.0f));
	m_camera->SetOrthographic(true);

	m_scriptSystem.BindToLua(*(Engine::GetInstance()->GetInput()));
	m_scriptSystem.BindToLua(*(Engine::GetInstance()->GetTime()));
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
		else if (name == "ActiveCamera")
		{
			m_activeCameraId = property.attribute("value").as_int();
		}
	}

	m_world = std::make_unique<PhysicsWorld>(glm::vec2{ 0.f, 0.f }, timeStep, velocityIterations, positionIterations, pixelsPerMeter);


	TileSheet tileSheet(Engine::GetInstance()->GetProjectDirectory(), fileDir + doc.child("map").child("tileset").attribute("source").as_string());

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

			tilemapEntity->CreateComponent<MaterialComponent>(tileSheet.GetTileSetImagePath(), "res/shaders/sprite.vert", "res/shaders/sprite.frag");
			tilemapEntity->CreateComponent<RenderComponent>(width * height);
			auto* tilemap = tilemapEntity->CreateComponent<TileMapComponent>(width, height, levelArray, tileSheet);
			auto* transform = tilemapEntity->CreateComponent<TransformComponent>(tilemapEntity, glm::vec3(), glm::vec3(), glm::vec3{ tileSheet.GetTileWidth(), tileSheet.GetTileHeight(), 1.f });
			
			for (auto& property : layer.child("properties").children())
			{
				std::string propertyName = property.attribute("name").as_string();
				if (propertyName == "ColliderType")
				{
					tilemapEntity->CreateComponent<TileMapCollisionBodyComponent>(m_world.get(), *tilemap);
				}
			}
		}
		else if (name == "objectgroup")
		{
			// create tilemap component
			auto* tilemapEntity = CreateEntity(layer.attribute("name").as_string());
			tilemapEntity->CreateComponent<TransformComponent>(tilemapEntity, glm::vec3{ layer.attribute("offsetx").as_float(), layer.attribute("offsety").as_float(), 0.f }, glm::vec3(), glm::vec3{1.f,1.f,1.f});

			for (auto& object : layer.children("object"))
			{
				auto* gameObjectEntity = CreateObject(object, fileDir, tileSheet);
				gameObjectEntity->SetParent(tilemapEntity);

				auto* transform = gameObjectEntity->GetComponent<TransformComponent>();
				auto* physicsBody = gameObjectEntity->GetComponent<PhysicsBodyComponent>();
				auto* staticBody = gameObjectEntity->GetComponent<StaticBodyComponent>();
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

	TileSheet tileSheet(Engine::GetInstance()->GetProjectDirectory(), workingDir + doc.child("template").child("tileset").attribute("source").as_string());

	return CreateObject(doc.child("template").child("object"), workingDir, tileSheet);
}

Entity* Scene::CreateObject(const pugi::xml_node& object, const std::string& fileDir, const TileSheet& tileSheet)
{
	Entity* gameObjectEntity = nullptr;

	glm::vec3 objectPosition{ object.attribute("x") ? object.attribute("x").as_float() : 0.f, object.attribute("y") ? object.attribute("y").as_float() - tileSheet.GetTileHeight() : 0.f, 0.f };

	if (object.attribute("template"))
	{
		gameObjectEntity = LoadTemplate(fileDir, object.attribute("template").as_string());
		auto* tranformComponent = gameObjectEntity->GetComponent<TransformComponent>();
		auto* physicsBody = gameObjectEntity->GetComponent<PhysicsBodyComponent>();
		auto* staticBody = gameObjectEntity->GetComponent<StaticBodyComponent>();
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

		gameObjectEntity->CreateComponent<MaterialComponent>(tileSheet.GetTileSetImagePath(), "res/shaders/sprite.vert", "res/shaders/sprite.frag");
		gameObjectEntity->CreateComponent<RenderComponent>(1u);
		gameObjectEntity->CreateComponent<SpriteComponent>(tileSheet, object.attribute("gid").as_uint() - 1);
		gameObjectEntity->CreateComponent<TransformComponent>(gameObjectEntity, objectPosition, glm::vec3(), glm::vec3{ tileSheet.GetTileWidth(), tileSheet.GetTileHeight(), 1.f });
	}

	std::vector<ScriptProperty> scriptProperties;
	std::unordered_map<std::string, SpriteAnimation> spriteAnimations;
	std::string defaultAnimation;
	std::string colliderType;
	float colliderWidth = 0.f;
	float colliderHeight = 0.f;

	for (auto& property : object.child("properties").children())
	{
		std::string propertyName = property.attribute("name").as_string();

		// Define the regex pattern
		std::regex pattern("#");

		// Use regex_iterator to split the string
		std::sregex_token_iterator iterator(propertyName.begin(), propertyName.end(), pattern, -1);
		std::sregex_token_iterator end;

		std::vector<std::string> splitPropertyName(iterator, end);

		if (splitPropertyName[0] == "Script")
		{
			if (splitPropertyName.size() > 1)
			{
				std::string type = property.attribute("type").as_string();
				sol::state& lua = m_scriptSystem.GetSolState();
				if (type == "float")
				{
					lua["temp"] = property.attribute("value").as_float();
				}
				else if (type == "int")
				{
					lua["temp"] = property.attribute("value").as_int();
				}
				else if (type == "bool")
				{
					lua["temp"] = property.attribute("value").as_bool();
				}
				else if (type == "string" || type == "file")
				{
					lua["temp"] = property.attribute("value").as_string();
				}
				scriptProperties.push_back({ splitPropertyName[1], lua["temp"] });
			}
			else
			{
				std::string script = property.attribute("value").as_string();
				gameObjectEntity->CreateComponent<ScriptComponent>(fileDir + script, m_scriptSystem.GetSolState(), gameObjectEntity);
			}
		}
		else if (splitPropertyName[0] == "ColliderType")
		{
			colliderType = property.attribute("value").as_string();
			ASSERT(colliderType == "Dynamic" || colliderType == "Static");
		}
		else if (splitPropertyName[0] == "Animation")
		{
			unsigned int startFrame, endFrame;
			float animationTime;
			bool loop = false;
			for (auto& animationProperty : property.child("properties").children())
			{
				std::string name = animationProperty.attribute("name").as_string();
				if (name == "startFrame")
				{
					startFrame = animationProperty.attribute("value").as_int();
				}
				else if (name == "endFrame")
				{
					endFrame = animationProperty.attribute("value").as_int();
				}
				else if (name == "loop")
				{
					loop = animationProperty.attribute("value").as_bool();
				}
				else if (name == "animationTime")
				{
					animationTime = animationProperty.attribute("value").as_float();
				}
			}
			// TODO check ll value are valid from the file
			SpriteAnimation spriteAnimation{ startFrame, endFrame, animationTime, loop };
			spriteAnimations[splitPropertyName[1]] = spriteAnimation;
		}
		else if (splitPropertyName[0] == "DefaultAnimation")
		{
			defaultAnimation = property.attribute("value").as_string();
		}
		else if (splitPropertyName[0] == "Sprite")
		{
			auto* sprite = gameObjectEntity->GetComponent<SpriteComponent>();
			if (!sprite) continue;
			for (auto& spriteProperty : property.child("properties").children())
			{
				std::string name = spriteProperty.attribute("name").as_string();
				if (name == "XOffset")
				{
					sprite->xOffset = spriteProperty.attribute("value").as_float();
				}
				else if(name == "YOffset")
				{
					sprite->yOffset = spriteProperty.attribute("value").as_float();
				}
			}
		}
		else if (splitPropertyName[0] == "Collider")
		{
			for (auto& colliderProperty : property.child("properties").children())
			{
				std::string name = colliderProperty.attribute("name").as_string();
				if (name == "Width")
				{
					colliderWidth = colliderProperty.attribute("value").as_float();
				}
				else if(name == "Height")
				{
					colliderHeight = colliderProperty.attribute("value").as_float();
				}
			}
		}
		else if (splitPropertyName[0] == "Camera")
		{
			auto* cameraComponent = gameObjectEntity->CreateComponent<CameraComponent>();
			// is camera active camera
			if (object.attribute("id").as_int() == m_activeCameraId)
			{
				cameraComponent->MakeCameraActive(m_camera.get());
				cameraComponent->SetForcePosition(true);
			}
		}
	}
	auto* scriptComponent = gameObjectEntity->GetComponent<ScriptComponent>();
	if (scriptComponent)
	{
		for (auto& scriptProperty : scriptProperties)
		{
			scriptComponent->SetScriptProperty(scriptProperty.name, scriptProperty.value);
		}
	}
	if (spriteAnimations.size())
	{
		gameObjectEntity->CreateComponent<SpriteAnimationComponent>(spriteAnimations, !defaultAnimation.empty() ? defaultAnimation : spriteAnimations.begin()->first);
	}
	if (colliderType != "")
	{
		auto* transform = gameObjectEntity->GetComponent<TransformComponent>();
		float width = colliderWidth ? colliderWidth : object.attribute("width").as_float();
		float height = colliderHeight ? colliderHeight : object.attribute("height").as_float();
		if (colliderType == "Dynamic")
		{
			auto* physicsBody = gameObjectEntity->CreateComponent<PhysicsBodyComponent>(m_world.get(), glm::vec2{ width, height });
			physicsBody->SetPosition(transform->GetWorldPosition());
		}
		else if (colliderType == "Static")
		{
			auto* staticsBody = gameObjectEntity->CreateComponent<StaticBodyComponent>(m_world.get(), glm::vec2{ width, height });
			staticsBody->SetPosition(transform->GetWorldPosition());
		}
	}

	return gameObjectEntity;
}

Entity* Scene::CreateTemplatedObject(const std::string& levelFile)
{
	return LoadTemplate(Engine::GetInstance()->GetProjectDirectory(), levelFile);
}

Entity* Scene::CreateEntity(const std::string& name)
{
	m_entities.push_back(std::make_unique<Entity>(name, &m_registry));
	return m_entities.back().get();
}

void Scene::Update()
{
	m_physicsSystem.Update(m_world.get());

	m_scriptSystem.Update();

	m_animationSystem.Update();

	m_cameraSystem.Update(m_camera.get());
}

void Scene::Render()
{
	GUIPanel::EntityHierarchy::Panel(m_entities);

	m_renderSystem.Update();

	//DebugRenderer::DrawRectangle({ 29.f * 8.f, 19.f * 8.f, 0.f }, 30.f * 16.f, 20.f * 16.f, {0.5f,0.5f,0.5f});

	glm::mat4 projection = m_camera->CalculateProjection((float)Engine::GetInstance()->GetWindow()->GetWidth(), (float)Engine::GetInstance()->GetWindow()->GetHeight());
	glm::mat4 view = m_camera->GetViewMatrix();
	DebugRenderer::Render(projection * view);
}

void Scene::LUABind(sol::state& lua)
{
	lua.new_usertype<Scene>("Scene",
		"createTemplatedObject", &Scene::CreateTemplatedObject
	);
	lua.set("Scene", this);
}

