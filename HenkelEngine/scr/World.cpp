#include "World.h"
#include "DebugGUIPanels\GUIPanels.h"
#include "ECS\System\AnimationSystem.h"
#include "ECS\System\PhysicsSystem.h"
#include "ECS\System\RenderSystem.h"
#include "ECS\System\ScriptSystem.h"
#include "ECS\System\CameraSystem.h"
#include "ECS\System\UISystem.h"
#include "ECS\System\LevelSystem.h"
#include "Engine.h"
#include "ECS\Component\LevelComponent.h"
#include "FileParser.h"

// TODO might want to load this from a file
constexpr static const float TIME_STEP = 1.0f / FPS;
constexpr static const int VELOCITY_ITERATIONS = 6;
constexpr static const int POSITION_ITERATIONS = 4;

World::World()
	: m_registry(), m_entities(), m_camera(std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 0.0f))), m_physicsWorld(std::make_unique<PhysicsWorld>(glm::vec2{ 0.f, 0.f }, TIME_STEP, VELOCITY_ITERATIONS, POSITION_ITERATIONS, 1.0f))
{
	
}

World::~World()
{
}

void World::Update()
{
	PhysicsSystem::Update(&m_registry, m_physicsWorld.get());
	ScriptSystem::Update(&m_registry, Engine::GetInstance()->GetSolState());
	AnimationSystem::Update(&m_registry);
	CameraSystem::Update(&m_registry, m_camera.get());
	UISystem::Update(&m_registry);
#ifdef _DEBUG
	LevelSystem::Update(&m_registry);
	GUIPanel::EntityHierarchy::Panel(m_entities);
#endif // _DEBUG
}

void World::Render()
{
	RenderSystem::Update(&m_registry);
}

Camera* World::GetCamera() const
{
	return m_camera.get();
}

PhysicsWorld* World::GetPhysicsWorld() const
{
	return m_physicsWorld.get();
}

Entity* World::CreateTemplatedObject(const std::string& levelFile)
{
	return FileParser::CreateTemplatedObject(this, levelFile);
}

Entity* World::CreateEntity(const std::string& name)
{
	m_entities.push_back(std::make_unique<Entity>(name, &m_registry));
	return m_entities.back().get();
}

void World::DeleteEntity(Entity* entity)
{
	for (auto& childEntity : entity->GetChildren())
	{
		DeleteEntity(childEntity);
	}
	m_registry.DeleteEntity(entity->GetEntityId());
	auto entityIter = std::find_if(m_entities.begin(), m_entities.end(), [entity](const std::unique_ptr<Entity>& entityPtr) { return entityPtr.get() == entity; });
	m_entities.erase(entityIter);
}

Entity* World::GetEntityFromEntityID(EntityId entityId)
{
	for (auto& entity : m_entities)
	{
		if (entity->GetEntityId() == entityId)
		{
			return entity.get();
		}
	}
	return nullptr;
}

template<typename ComponentType>
sol::table LUAGetAllObjectsOfType(World* world)
{
	std::vector<Entity*> entities = world->GetEntitiesWithComponent<ComponentType>();
	sol::state& lua = Engine::GetInstance()->GetInstance()->GetSolState();
	sol::table objects = lua.create_table();
	for (auto entity : entities)
	{
		objects.add(entity);
	}
	return objects;
}

void World::LUABind(sol::state& lua)
{
	lua.new_usertype<World>("World"
		, "loadLevel", [](World* world, Entity* levelEntity) { LevelSystem::LoadLevel(world, levelEntity); }
		, "unloadLevel", [](World* world, Entity* levelEntity) { LevelSystem::UnloadLevel(world, levelEntity); }
		, "findLevelWithPosition", [](World* world, const glm::vec2& position) { return LevelSystem::FindLevelWithPosition(world, &world->m_registry, position); }
		, "getLevelEntities", &LUAGetAllObjectsOfType<LevelComponent>
	);
	lua.set("World", this);
}
