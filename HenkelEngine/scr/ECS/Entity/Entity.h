#pragma once

#include "entt.hpp"
#include <string>
#include "sol\sol.hpp"
#include <ECS\Component\TransformComponent.h>
#include <ECS\Component\PhysicsBodyComponents\PhysicsBodyComponent.h>

class Entity
{
public:

	Entity(const std::string& name, entt::registry* registry);

	entt::entity GetEntity() const;

	template <typename Type, typename... Args>
	Type* CreateComponent(Args &&...args)
	{
		return &m_registry->emplace<Type>(m_entity, std::forward<Args>(args)...);
	}	
	
	template <typename Type>
	Type* GetComponent()
	{
		return m_registry->try_get<Type>(m_entity);
	}

	static void LUABind(sol::state& lua);

private:
	entt::entity m_entity;
	entt::registry* m_registry;

	std::string m_name;
};

