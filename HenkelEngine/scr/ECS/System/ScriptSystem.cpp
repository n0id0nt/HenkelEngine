#include "ScriptSystem.h"
#include "ECS\Component\TransformComponent.h"
#include "ECS\Component\PhysicsBodyComponents\PhysicsBodyComponent.h"
#include "ECS/Component/ScriptComponent.h"
#include "HelperFunctions.h"
#include "sol/sol.hpp"
#include <opengl\openglHelper.h>
#include "ECS\Entity\Entity.h"

ScriptSystem::ScriptSystem(Registry* registry) : m_registry(registry), m_lua()
{
	m_lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math);
	HenkelEngine::LUABindLibraries(m_lua);
	Entity::LUABind(m_lua);
	TransformComponent::LUABind(m_lua);
	PhysicsBodyComponent::LUABind(m_lua);
	ScriptComponent::LUABind(m_lua);
}

void ScriptSystem::BindToLua(LUABindable& luaBindable)
{
	luaBindable.LUABind(m_lua);
}

void ScriptSystem::Update(float deltaTime)
{
	{
		auto view = m_registry->GetEntitiesWithComponent<ScriptComponent>();
		for (auto& entity : view)
		{
			auto* script = m_registry->GetComponent<ScriptComponent>(entity);
			script->Bind(m_lua);
			script->Update(deltaTime);
			script->Unbind(m_lua);
		}
	}
}

sol::state& ScriptSystem::GetSolState()
{
	return m_lua;
}
