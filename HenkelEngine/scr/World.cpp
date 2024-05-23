#include "World.h"
#include "DebugGUIPanels\GUIPanels.h"
#include "ECS\System\AnimationSystem.h"
#include "ECS\System\PhysicsSystem.h"
#include "ECS\System\RenderSystem.h"
#include "ECS\System\ScriptSystem.h"
#include "ECS\System\CameraSystem.h"
#include "ECS\System\UISystem.h"
#include "Engine.h"

// TODO might want to load this from a file
constexpr static const float TIME_STEP = 1.0f / FPS;
constexpr static const int VELOCITY_ITERATIONS = 40;
constexpr static const int POSITION_ITERATIONS = 30;

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
}

void World::Render()
{
#ifdef _DEBUG
	GUIPanel::EntityHierarchy::Panel(m_entities);
#endif // _DEBUG

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
	return nullptr;
}

Entity* World::CreateEntity(const std::string& name)
{
	m_entities.push_back(std::make_unique<Entity>(name, &m_registry));
	return m_entities.back().get();
}

void World::DeleteEntity(Entity* entity)
{
	ASSERT(false) //not implemented yet
		// remove entity from the list and also need to remove all the components from the registry and ensure all resources the components used are released and remove collider from physics scene
}

void World::LUABind(sol::state& lua)
{
	lua.new_usertype<World>("World"
		//,"setCurrentLevel", &World::SetCurrentLevel
	);
	lua.set("World", this);
}
