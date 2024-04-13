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
#include "ECS\Component\UIComponent.h"
#include <imgui.h>
#include <ECS\Component\TransformComponent.h>
#include "DebugGUIPanels\GUIPanels.h"
#include <ECS\Component\CameraComponent.h>
#include "UI/UIArea.h"
#include "UI/UIQuad.h"
#include "UI/UITexture.h"
#include "UI/UIText.h"

const float TIME_STEP = 1.0f / FPS;
const int VELOCITY_ITERATIONS = 40;
const int POSITION_ITERATIONS = 30;


Scene::Scene(const std::string& fileDir, const std::string& levelFile) 
	: m_name("Scene"), m_registry(), m_animationSystem(&m_registry), m_physicsSystem(&m_registry), m_renderSystem(&m_registry), m_scriptSystem(&m_registry), m_cameraSystem(&m_registry), m_entities(), m_uiSystem(&m_registry)
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

	m_world = std::make_unique<PhysicsWorld>(glm::vec2{ 0.f, 0.f }, TIME_STEP, VELOCITY_ITERATIONS, POSITION_ITERATIONS, pixelsPerMeter);
	m_world->SetContactListener(m_physicsSystem.GetContactListener());


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

			std::vector<ScriptProperty> scriptProperties;


			// retrieve level data
			ASSERT(std::string(layer.child("data").attribute("encoding").as_string()) == "base64");
			std::string base64Data = layer.child("data").text().as_string();
			std::string data = base64_decode(HenkelEngine::trim(base64Data));
			uLongf numGids = width * height * sizeof(int);
			std::vector<unsigned> levelArray(numGids);
			uncompress((Bytef*)levelArray.data(), &numGids, (const Bytef*)data.c_str(), data.size());
			levelArray.erase(levelArray.begin() + width * height, levelArray.end());

			tilemapEntity->CreateComponent<MaterialComponent>("res/shaders/sprite.vert", "res/shaders/sprite.frag");
			tilemapEntity->CreateComponent<RenderComponent>(width * height);
			auto* tilemap = tilemapEntity->CreateComponent<TileMapComponent>(width, height, levelArray, tileSheet);
			auto* transform = tilemapEntity->CreateComponent<TransformComponent>(tilemapEntity, glm::vec3(), glm::vec3(), glm::vec3{ tileSheet.GetTileWidth(), tileSheet.GetTileHeight(), 1.f });
			
			for (auto& property : layer.child("properties").children())
			{
				std::string propertyName = property.attribute("name").as_string();
				// Define the regex pattern
				std::regex pattern("#");

				// Use regex_iterator to split the string
				std::sregex_token_iterator iterator(propertyName.begin(), propertyName.end(), pattern, -1);
				std::sregex_token_iterator end;

				std::vector<std::string> splitPropertyName(iterator, end);

				if (propertyName == "Collider")
				{
					std::string colliderType = property.attribute("value").as_string();
					bool isSensor = false;
					uint16 categoryLayer = Engine::GetInstance()->GetCollisionLayer("All");
					uint16 maskLayer = Engine::GetInstance()->GetCollisionLayer("All");
					for (auto& colliderProperty : property.child("properties").children())
					{
						std::string name = colliderProperty.attribute("name").as_string();
						if (name == "Sensor")
						{
							isSensor = colliderProperty.attribute("value").as_bool();
						}
						else if (name == "Category")
						{
							categoryLayer = Engine::GetInstance()->GetCollisionLayer(colliderProperty.attribute("value").as_string());
						}
						else if (name == "Mask")
						{
							maskLayer = Engine::GetInstance()->GetCollisionLayer(colliderProperty.attribute("value").as_string());
						}
					}
					tilemapEntity->CreateComponent<TileMapCollisionBodyComponent>(m_world.get(), *tilemap, tilemapEntity, isSensor, categoryLayer, maskLayer);
				}
				else if (splitPropertyName[0] == "Tag")
				{
					tilemapEntity->AddTag(splitPropertyName[1]);
				}
				else if (splitPropertyName[0] == "Script")
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
						tilemapEntity->CreateComponent<ScriptComponent>(fileDir + script, m_scriptSystem.GetSolState(), tilemapEntity);
					}
				}
			}
			auto* scriptComponent = tilemapEntity->GetComponent<ScriptComponent>();
			if (scriptComponent)
			{
				for (auto& scriptProperty : scriptProperties)
				{
					scriptComponent->SetScriptProperty(scriptProperty.name, scriptProperty.value);
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

	Entity* uiEntity = CreateEntity("UIArea");
	uiEntity->CreateComponent<TransformComponent>(uiEntity, glm::vec3(), glm::vec3(), glm::vec3{ 1.0f, 1.0f, 1.0f });
	uiEntity->CreateComponent<MaterialComponent>("res/shaders/UI.vert", "res/shaders/UI.frag"); //TODO define these values here as constants
	uiEntity->CreateComponent<RenderComponent>(6000u);
	UIComponent* uiComponent = uiEntity->CreateComponent<UIComponent>();
	uiComponent->GetRootArea()->SetDimensions(glm::vec2(1.f, 1.f));
	std::unique_ptr<UITexture> quad = std::make_unique<UITexture>();
	quad->SetColor(glm::vec4(0.6f, 0.3f, 0.5f, 0.6f));
	quad->SetTexture("res/images/CubeFace.png");
	quad->SetPosition(glm::vec2(100.0f, 220.3f));
	quad->SetDimensions(glm::vec2(220.6f, 110.3f));
	
	std::unique_ptr<UIQuad> quad2 = std::make_unique<UIQuad>();
	quad2->SetColor(glm::vec4(0.2f, 0.5f, 0.7f, 0.6f));
	quad2->SetPosition(glm::vec2(20.6f, 20.3f));
	quad2->SetDimensions(glm::vec2(20.6f, 20.3f));
	quad->AddChild(std::move(quad2));
	
	std::unique_ptr<UITexture> quad3 = std::make_unique<UITexture>();
	//quad3->SetColor(glm::vec4(1.f, 1.f, 1.f, 1.f));
	quad3->SetPosition(glm::vec2(60.6f, 20.3f));
	quad3->SetDimensions(glm::vec2(20.6f, 20.3f));
	quad3->SetTexture("res/images/Zombie.png");
	quad->AddChild(std::move(quad3));
	
	std::unique_ptr<UIQuad> quad4 = std::make_unique<UIQuad>();
	quad4->SetColor(glm::vec4(0.2f, 0.5f, 0.7f, 0.6f));
	quad4->SetPosition(glm::vec2(100.6f, 20.3f));
	quad4->SetDimensions(glm::vec2(20.6f, 20.3f));
	quad->AddChild(std::move(quad4));
	
	std::unique_ptr<UIQuad> quad5 = std::make_unique<UIQuad>();
	quad5->SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	quad5->SetPosition(glm::vec2(20.6f, 50.3f));
	quad5->SetDimensions(glm::vec2(185.6f, 200.3f));
	quad->AddChild(std::move(quad5));
	
	std::unique_ptr<UIText> quad6 = std::make_unique<UIText>();
	//quad5->SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	quad6->SetPosition(glm::vec2(20.6f, 50.3f));
	quad6->SetDimensions(glm::vec2(185.6f, 200.3f));
	quad6->SetText("!The Quick Brown Fox Jumps Over The Lazy Dog!");
	quad6->SetFont("../Example/fonts/arial.ttf", 20);
	quad->AddChild(std::move(quad6));
	
	uiComponent->GetRootArea()->AddChild(std::move(quad));
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

		gameObjectEntity->CreateComponent<MaterialComponent>("res/shaders/sprite.vert", "res/shaders/sprite.frag");
		gameObjectEntity->CreateComponent<RenderComponent>(1u);
		gameObjectEntity->CreateComponent<SpriteComponent>(tileSheet, object.attribute("gid").as_uint() - 1);
		gameObjectEntity->CreateComponent<TransformComponent>(gameObjectEntity, objectPosition, glm::vec3(), glm::vec3{ tileSheet.GetTileWidth(), tileSheet.GetTileHeight(), 1.f });
	}

	std::vector<ScriptProperty> scriptProperties;
	std::unordered_map<std::string, SpriteAnimation> spriteAnimations;
	std::string defaultAnimation;
	std::string colliderType;
	bool isSensor = false;
	float colliderWidth = 0.f;
	float colliderHeight = 0.f;
	uint16 categoryLayer = Engine::GetInstance()->GetCollisionLayer("All");
	uint16 maskLayer = Engine::GetInstance()->GetCollisionLayer("All");

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
		else if (splitPropertyName[0] == "Collider")
		{
			colliderType = "Static";
			for (auto& colliderProperty : property.child("properties").children())
			{
				std::string name = colliderProperty.attribute("name").as_string();
				if (name == "Sensor")
				{
					isSensor = colliderProperty.attribute("value").as_bool();
				}
				else if (name == "ColliderType")
				{
					colliderType = colliderProperty.attribute("value").as_string();
				}
				else if (name == "ColliderShape")
				{
					for (auto& colliderShapeProperty : colliderProperty.child("properties").children())
					{
						std::string shapeName = colliderShapeProperty.attribute("name").as_string();
						if (shapeName == "Width")
						{
							colliderWidth = colliderShapeProperty.attribute("value").as_float();
						}
						else if(shapeName == "Height")
						{
							colliderHeight = colliderShapeProperty.attribute("value").as_float();
						}
					}
				}
				else if (name == "Category")
				{
					categoryLayer = Engine::GetInstance()->GetCollisionLayer(colliderProperty.attribute("value").as_string());
				}
				else if (name == "Mask")
				{
					maskLayer = Engine::GetInstance()->GetCollisionLayer(colliderProperty.attribute("value").as_string());
				}
			}
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
		else if (splitPropertyName[0] == "Tag")
		{
			gameObjectEntity->AddTag(splitPropertyName[1]);
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
		else if (splitPropertyName[0] == "Camera")
		{
			auto* cameraComponent = gameObjectEntity->CreateComponent<CameraComponent>();
			// is camera active camera
			if (object.attribute("id").as_int() == m_activeCameraId)
			{
				cameraComponent->MakeCameraActiveFromCamera(m_camera.get());
				cameraComponent->ForcePosition();
			}
			for (auto& cameraProperty : property.child("properties").children())
			{
				std::string name = cameraProperty.attribute("name").as_string();
				if (name == "Zoom")
				{
					cameraComponent->SetZoom(cameraProperty.attribute("value").as_float());
				}
				else if (name == "DeadZone")
				{
					glm::vec2 vec{};
					for (auto& vecProperty : cameraProperty.child("properties").children())
					{
						std::string name = vecProperty.attribute("name").as_string();
						if (name == "X")
						{
							vec.x = vecProperty.attribute("value").as_float();
						}
						else if (name == "Y")
						{
							vec.y = vecProperty.attribute("value").as_float();
						}
					}
					cameraComponent->SetDeadZone(vec);
				}
				else if (name == "Offset")
				{
					glm::vec2 vec{};
					for (auto& vecProperty : cameraProperty.child("properties").children())
					{
						std::string name = vecProperty.attribute("name").as_string();
						if (name == "X")
						{
							vec.x = vecProperty.attribute("value").as_float();
						}
						else if (name == "Y")
						{
							vec.y = vecProperty.attribute("value").as_float();
						}
					}
					cameraComponent->SetOffset(vec);
				}
				else if (name == "Damping")
				{
					glm::vec2 vec{};
					for (auto& vecProperty : cameraProperty.child("properties").children())
					{
						std::string name = vecProperty.attribute("name").as_string();
						if (name == "X")
						{
							vec.x = vecProperty.attribute("value").as_float();
						}
						else if (name == "Y")
						{
							vec.y = vecProperty.attribute("value").as_float();
						}
					}
					cameraComponent->SetDamping(vec);
				}
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
			auto* physicsBody = gameObjectEntity->CreateComponent<PhysicsBodyComponent>(m_world.get(), glm::vec2{ width, height }, gameObjectEntity, isSensor, categoryLayer, maskLayer);
			physicsBody->SetPosition(transform->GetWorldPosition());
		}
		else if (colliderType == "Static")
		{
			auto* staticsBody = gameObjectEntity->CreateComponent<StaticBodyComponent>(m_world.get(), glm::vec2{ width, height }, gameObjectEntity, isSensor, categoryLayer, maskLayer);
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
	m_uiSystem.Update();
}

void Scene::Render()
{
	GUIPanel::EntityHierarchy::Panel(m_entities);

	m_renderSystem.Update();
}

sol::state& Scene::GetLuaState()
{
	return m_scriptSystem.GetSolState();
}

void Scene::LUABind(sol::state& lua)
{
	lua.new_usertype<Scene>("Scene",
		"createTemplatedObject", &Scene::CreateTemplatedObject
	);
	lua.set("Scene", this);
}

