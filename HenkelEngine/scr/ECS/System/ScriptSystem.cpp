#include "ScriptSystem.h"
#include "ECS\Component\TransformComponent.h"
#include "ECS\Component\PhysicsBodyComponents\PhysicsBodyComponent.h"
#include "ECS/Component/ScriptComponent.h"
#include "HelperFunctions.h"
#include "sol/sol.hpp"
#include <opengl\openglHelper.h>
#include "ECS\Entity\Entity.h"

ScriptSystem::ScriptSystem(entt::registry* registry) : m_registry(registry), m_lua()
{
	m_lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math);
	HenkelEngine::LUABindLibraries(m_lua);
	Entity::LUABind(m_lua);
	TransformComponent::LUABind(m_lua);
	PhysicsBodyComponent::LUABind(m_lua);
}

void ScriptSystem::BindToLua(LUABindable& luaBindable)
{
	luaBindable.LUABind(m_lua);
}

void ScriptSystem::Update(float deltaTime)
{
	{
		auto view = m_registry->view<ScriptComponent>();
		for (auto& entity : view)
		{
			auto& script = view.get<ScriptComponent>(entity);
			script.Bind(m_lua);
			script.Update(deltaTime);
			script.Unbind(m_lua);
		}
	}
}

void ScriptSystem::CreateScriptComponent(Entity* entity, const std::string& file)
{
	m_registry->emplace<ScriptComponent>(entity->GetEntity(), file, m_lua, entity, m_registry);
}
