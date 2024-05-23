#include "ScriptSystem.h"
#include "ECS\Component\TransformComponent.h"
#include "ECS\Component\PhysicsBodyComponents\CollisionBodyComponent.h"
#include "ECS\Component\PhysicsBodyComponents\PhysicsBodyComponent.h"
#include "ECS\Component\PhysicsBodyComponents\StaticBodyComponent.h"
#include "ECS\Component\PhysicsBodyComponents\TileMapCollisionBodyComponent.h"
#include "ECS/Component/ScriptComponent.h"
#include "ECS/Component/RenderComponents/SpriteComponent.h"
#include "ECS/Component/RenderComponents/RenderComponent.h"
#include "HelperFunctions.h"
#include "sol/sol.hpp"
#include <opengl\openglHelper.h>
#include "ECS\Entity\Entity.h"
#include <ECS\Component\SpriteAnimationComponent.h>
#include <ECS\Component\CameraComponent.h>
#include <ECS\Component\UIComponent.h>
#include <format>
#include <Engine.h>
#include "UI\UIArea.h"
#include "UI\UIText.h"

void ScriptSystem::Update(Registry* registry, sol::state& lua)
{
	auto scriptComponents = registry->GetAllComponents<ScriptComponent>();
	for (auto& script : scriptComponents)
	{
		script->Bind(lua);
		script->Update();
		script->Unbind(lua);
	}
}
