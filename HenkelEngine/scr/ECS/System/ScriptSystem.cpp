#include "ScriptSystem.h"
#include "ECS\Component\TransformComponent.h"
#include "ECS\Component\PhysicsBodyComponents\PhysicsBodyComponent.h"
#include "ECS/Component/ScriptComponent.h"
#include "ECS/Component/RenderComponents/SpriteComponent.h"
#include "HelperFunctions.h"
#include "sol/sol.hpp"
#include <opengl\openglHelper.h>
#include "ECS\Entity\Entity.h"
#include <ECS\Component\SpriteAnimationComponent.h>
#include <ECS\Component\CameraComponent.h>
#include <format>
#include <Engine.h>

ScriptSystem::ScriptSystem(Registry* registry) : m_registry(registry), m_lua()
{
	m_lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math);
	// Change the working directory
	m_lua.script(std::format("package.path = '{}Scripts/?.lua'", Engine::GetInstance()->GetProjectDirectory()));

	HenkelEngine::LUABindLibraries(m_lua);
	Entity::LUABind(m_lua);
	TransformComponent::LUABind(m_lua);
	PhysicsBodyComponent::LUABind(m_lua);
	ScriptComponent::LUABind(m_lua);
	SpriteComponent::LUABind(m_lua);
	SpriteAnimationComponent::LUABind(m_lua);
	CameraComponent::LUABind(m_lua);
}

void ScriptSystem::BindToLua(LUABindable& luaBindable)
{
	luaBindable.LUABind(m_lua);
}

void ScriptSystem::Update()
{
	{
		auto view = m_registry->GetEntitiesWithComponent<ScriptComponent>();
		for (auto& entity : view)
		{
			auto* script = m_registry->GetComponent<ScriptComponent>(entity);
			script->Bind(m_lua);
			script->Update();
			script->Unbind(m_lua);
		}
	}
}

sol::state& ScriptSystem::GetSolState()
{
	return m_lua;
}
