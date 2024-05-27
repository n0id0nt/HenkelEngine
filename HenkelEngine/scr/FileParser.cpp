#include "FileParser.h"
#include "Engine.h"
#include "pugixml.hpp"
#include "base64.h"
#include "HelperFunctions.h"
#include "zlib.h"
#include "ECS\Component\RenderComponents\RenderComponent.h"
#include "ECS/Component/LevelComponent.h"
#include "ECS\Component\ScriptComponent.h"
#include "ECS\Component\RenderComponents\TileMapComponent.h"
#include "ECS\Component\PhysicsBodyComponents\TileMapCollisionBodyComponent.h"
#include "ECS\Component\RenderComponents\SpriteComponent.h"
#include "ECS\Component\PhysicsBodyComponents\StaticBodyComponent.h"
#include "ECS\Component\SpriteAnimationComponent.h"
#include "ECS\Component\UIComponent.h"
#include "UI/UIArea.h"
#include "UI/UIQuad.h"
#include "UI/UITexture.h"
#include "UI/UIText.h"

const std::string s_testMap1 = "TestLevel.tmx";
const std::string s_testMap2 = "AutoMappingTestLevel.tmx";

static int activeCameraId = 0;

void FileParser::LoadWorld(World* world, const std::string& fileDir, const std::string& worldFile)
{
	// this is a place holder and will adventually load from file
	world->GetPhysicsWorld()->SetPixelsPerMeter(16.0f);
	Entity* levelEntity0 = world->CreateEntity("AutoMappingTestLevel1.tmx");
	levelEntity0->CreateComponent<LevelComponent>("AutoMappingTestLevel.tmx", glm::ivec2(512, 256));
	levelEntity0->CreateComponent<TransformComponent>(levelEntity0, glm::vec3(1024, 16, 0));
	Entity* levelEntity1 = world->CreateEntity("AutoMappingTestLevel2.tmx");
	levelEntity1->CreateComponent<LevelComponent>("AutoMappingTestLevel2.tmx", glm::ivec2(512, 512));
	levelEntity1->CreateComponent<TransformComponent>(levelEntity1, glm::vec3(1536, -32, 0));
	Entity* levelEntity2 = world->CreateEntity("AutoMappingTestLevel3.tmx");
	levelEntity2->CreateComponent<LevelComponent>("AutoMappingTestLevel3.tmx", glm::ivec2(512, 256));
	levelEntity2->CreateComponent<TransformComponent>(levelEntity2, glm::vec3(2048, 64, 0));
	Entity* levelEntity3 = world->CreateEntity("AutoMappingTestLevel4.tmx");
	levelEntity3->CreateComponent<LevelComponent>("AutoMappingTestLevel4.tmx", glm::ivec2(512, 512));
	levelEntity3->CreateComponent<TransformComponent>(levelEntity3, glm::vec3(2384, 528, 0));

	LoadLevel(world, levelEntity0);
}

void FileParser::LoadLevel(World* world, Entity* levelEntity)
{
	LevelComponent* level = levelEntity->GetComponent<LevelComponent>();
	ASSERT(level);

	std::string fileDir = Engine::GetInstance()->GetProjectDirectory();
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file((fileDir + level->fileName).c_str());
	ASSERT(result);

	// level information to know how to read the file
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
			activeCameraId = property.attribute("value").as_int();
		}
	}

	TileSheet tileSheet(Engine::GetInstance()->GetProjectDirectory(), fileDir + doc.child("map").child("tileset").attribute("source").as_string());

	for (auto& layer : doc.child("map").children())
	{
		std::string name(layer.name());
		if (name == "layer")
		{
			// create tilemap component
			Entity* tilemapEntity = world->CreateEntity(layer.attribute("name").as_string());
			tilemapEntity->SetParent(levelEntity);
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
					auto* tilemapCollision = tilemapEntity->CreateComponent<TileMapCollisionBodyComponent>(world->GetPhysicsWorld(), *tilemap, tilemapEntity, isSensor, categoryLayer, maskLayer);
					tilemapCollision->SetPosition(transform->GetWorldPosition());
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
						sol::state& lua = Engine::GetInstance()->GetSolState();
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
						tilemapEntity->CreateComponent<ScriptComponent>(fileDir + script, Engine::GetInstance()->GetSolState(), tilemapEntity);
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
			Entity* objectGroupEntity = world->CreateEntity(layer.attribute("name").as_string());
			objectGroupEntity->SetParent(levelEntity);
			objectGroupEntity->CreateComponent<TransformComponent>(objectGroupEntity, glm::vec3{ layer.attribute("offsetx").as_float(), layer.attribute("offsety").as_float(), 0.f }, glm::vec3(), glm::vec3{ 1.f,1.f,1.f });

			for (auto& object : layer.children("object"))
			{
				Entity* gameObjectEntity = CreateObject(world, object, fileDir, tileSheet);
				gameObjectEntity->SetParent(objectGroupEntity);

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

	Entity* layoutEntity = LoadUILayout(world, fileDir, "UI/Layouts/TestLayout.xml");
	layoutEntity->SetParent(levelEntity);
	layoutEntity->CreateComponent<ScriptComponent>(fileDir + "Scripts/UI/UIScript.lua", Engine::GetInstance()->GetSolState(), layoutEntity);
}

Entity* FileParser::LoadTemplate(World* world, const std::string& fileDir, const std::string& levelFile)
{
	// Get Working Dir of new file
	std::string file = fileDir + levelFile;
	std::string workingDir = HenkelEngine::GetFileDir(file);
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(file.c_str());
	ASSERT(result);

	TileSheet tileSheet(Engine::GetInstance()->GetProjectDirectory(), workingDir + doc.child("template").child("tileset").attribute("source").as_string());

	return CreateObject(world, doc.child("template").child("object"), workingDir, tileSheet);
}

Entity* FileParser::CreateObject(World* world, const pugi::xml_node& object, const std::string& fileDir, const TileSheet& tileSheet)
{
	Entity* gameObjectEntity = nullptr;

	glm::vec3 objectPosition{ object.attribute("x") ? object.attribute("x").as_float() : 0.f, object.attribute("y") ? object.attribute("y").as_float() - tileSheet.GetTileHeight() : 0.f, 0.f };

	if (object.attribute("template"))
	{
		gameObjectEntity = LoadTemplate(world, fileDir, object.attribute("template").as_string());
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
		gameObjectEntity = world->CreateEntity(objectName);

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
				sol::state& lua = Engine::GetInstance()->GetSolState();
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
				gameObjectEntity->CreateComponent<ScriptComponent>(fileDir + script, Engine::GetInstance()->GetSolState(), gameObjectEntity);
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
						else if (shapeName == "Height")
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
				else if (name == "YOffset")
				{
					sprite->yOffset = spriteProperty.attribute("value").as_float();
				}
			}
		}
		else if (splitPropertyName[0] == "Camera")
		{
			auto* cameraComponent = gameObjectEntity->CreateComponent<CameraComponent>();
			// is camera active camera
			if (object.attribute("id").as_int() == activeCameraId)
			{
				cameraComponent->MakeCameraActiveFromCamera(world->GetCamera());
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
			auto* physicsBody = gameObjectEntity->CreateComponent<PhysicsBodyComponent>(world->GetPhysicsWorld(), glm::vec2{width, height}, gameObjectEntity, isSensor, categoryLayer, maskLayer);
			physicsBody->SetPosition(transform->GetWorldPosition());
		}
		else if (colliderType == "Static")
		{
			auto* staticsBody = gameObjectEntity->CreateComponent<StaticBodyComponent>(world->GetPhysicsWorld(), glm::vec2{ width, height }, gameObjectEntity, isSensor, categoryLayer, maskLayer);
			staticsBody->SetPosition(transform->GetWorldPosition());
		}
	}

	return gameObjectEntity;
}

Entity* FileParser::CreateTemplatedObject(World* world, const std::string& levelFile)
{
	return LoadTemplate(world, Engine::GetInstance()->GetProjectDirectory(), levelFile);
}

Entity* FileParser::LoadUILayout(World* world, const std::string& fileDir, const std::string& levelFile)
{
	// Get Working Dir of new file
	std::string file = fileDir + levelFile;
	std::string workingDir = HenkelEngine::GetFileDir(file);
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(file.c_str());
	ASSERT(result);

	Entity* uiEntity = world->CreateEntity("UILayout");
	uiEntity->CreateComponent<TransformComponent>(uiEntity, glm::vec3(), glm::vec3(), glm::vec3{ 1.0f, 1.0f, 1.0f });
	uiEntity->CreateComponent<MaterialComponent>("res/shaders/UI.vert", "res/shaders/UI.frag"); //TODO define these values here as constants
	uiEntity->CreateComponent<RenderComponent>(6000u);
	UIComponent* uiComponent = uiEntity->CreateComponent<UIComponent>();

	uiComponent->GetRootArea()->AddChild(CreateUIElement(world, doc.child("area"), workingDir));

	return uiEntity;
}

std::unique_ptr<UIArea> FileParser::CreateUIElement(World* world, const pugi::xml_node& object, const std::string& fileDir)
{
	std::unique_ptr<UIArea> uiElement;
	std::string uiElementType = object.name();

	if (uiElementType == "area")
	{
		std::unique_ptr<UIArea> area = std::make_unique<UIArea>();
		for (auto& attribute : object.attributes())
		{
			std::string attributeName = attribute.name();
			if (attributeName == "x")
			{
				area->SetX(attribute.as_float());
			}
			else if (attributeName == "y")
			{
				area->SetY(attribute.as_float());
			}
			else if (attributeName == "width")
			{
				area->SetWidth(attribute.as_float());
			}
			else if (attributeName == "height")
			{
				area->SetHeight(attribute.as_float());
			}
			else if (attributeName == "name")
			{
				area->SetName(attribute.as_string());
			}
		}

		for (auto& child : object.children())
		{
			area->AddChild(CreateUIElement(world, child, fileDir));
		}

		uiElement = std::move(area);
	}
	else if (uiElementType == "quad")
	{
		std::unique_ptr<UIQuad> area = std::make_unique<UIQuad>();
		for (auto& attribute : object.attributes())
		{
			std::string attributeName = attribute.name();
			if (attributeName == "x")
			{
				area->SetX(attribute.as_float());
			}
			else if (attributeName == "y")
			{
				area->SetY(attribute.as_float());
			}
			else if (attributeName == "width")
			{
				area->SetWidth(attribute.as_float());
			}
			else if (attributeName == "height")
			{
				area->SetHeight(attribute.as_float());
			}
			else if (attributeName == "name")
			{
				area->SetName(attribute.as_string());
			}
			else if (attributeName == "color")
			{
				std::string colorCode = attribute.as_string();
				glm::vec4 color = HenkelEngine::HexColorToVec4(colorCode);
				area->SetColor(color);
			}
		}

		for (auto& child : object.children())
		{
			area->AddChild(CreateUIElement(world, child, fileDir));
		}

		uiElement = std::move(area);
	}
	else if (uiElementType == "texture")
	{
		std::unique_ptr<UITexture> area = std::make_unique<UITexture>();
		for (auto& attribute : object.attributes())
		{
			std::string attributeName = attribute.name();
			if (attributeName == "x")
			{
				area->SetX(attribute.as_float());
			}
			else if (attributeName == "y")
			{
				area->SetY(attribute.as_float());
			}
			else if (attributeName == "width")
			{
				area->SetWidth(attribute.as_float());
			}
			else if (attributeName == "height")
			{
				area->SetHeight(attribute.as_float());
			}
			else if (attributeName == "name")
			{
				area->SetName(attribute.as_string());
			}
			else if (attributeName == "source")
			{
				area->SetTexture(attribute.as_string());
			}
			else if (attributeName == "color")
			{
				std::string colorCode = attribute.as_string();
				glm::vec4 color = HenkelEngine::HexColorToVec4(colorCode);
				area->SetColor(color);
			}
		}

		for (auto& child : object.children())
		{
			area->AddChild(CreateUIElement(world, child, fileDir));
		}

		uiElement = std::move(area);
	}
	else if (uiElementType == "text")
	{
		std::unique_ptr<UIText> area = std::make_unique<UIText>();
		for (auto& attribute : object.attributes())
		{
			std::string attributeName = attribute.name();
			if (attributeName == "x")
			{
				area->SetX(attribute.as_float());
			}
			else if (attributeName == "y")
			{
				area->SetY(attribute.as_float());
			}
			else if (attributeName == "width")
			{
				area->SetWidth(attribute.as_float());
			}
			else if (attributeName == "height")
			{
				area->SetHeight(attribute.as_float());
			}
			else if (attributeName == "name")
			{
				area->SetName(attribute.as_string());
			}
			else if (attributeName == "font")
			{
				area->SetFont(attribute.as_string());
			}
			else if (attributeName == "size")
			{
				area->SetSize(attribute.as_int());
			}
			else if (attributeName == "text")
			{
				area->SetText(attribute.as_string());
			}
			else if (attributeName == "alignment")
			{
				std::string attributeStringValue = attribute.as_string();
				if (attributeStringValue == "center")
				{
					area->SetTextAlignment(TextHorizontalAlignment::Center);
				}
				else if (attributeStringValue == "left")
				{
					area->SetTextAlignment(TextHorizontalAlignment::Left);
				}
				else if (attributeStringValue == "right")
				{
					area->SetTextAlignment(TextHorizontalAlignment::Right);
				}
				else
				{
					ASSERT(false);
				}
			}
			else if (attributeName == "wrapping")
			{
				std::string attributeStringValue = attribute.as_string();
				if (attributeStringValue == "overflow")
				{
					area->SetTextWrapping(TextWrapping::Overflow);
				}
				else if (attributeStringValue == "wrap")
				{
					area->SetTextWrapping(TextWrapping::Wrap);
				}
				else if (attributeStringValue == "cutoff")
				{
					area->SetTextWrapping(TextWrapping::CutOff);
				}
				else if (attributeStringValue == "ellipsis")
				{
					area->SetTextWrapping(TextWrapping::Ellipsis);
				}
				else
				{
					ASSERT(false);
				}
			}
			else if (attributeName == "color")
			{
				std::string colorCode = attribute.as_string();
				glm::vec4 color = HenkelEngine::HexColorToVec4(colorCode);
				area->SetColor(color);
			}
		}

		for (auto& child : object.children())
		{
			area->AddChild(CreateUIElement(world, child, fileDir));
		}

		uiElement = std::move(area);
	}

	return std::move(uiElement);
}
