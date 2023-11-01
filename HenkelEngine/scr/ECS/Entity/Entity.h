#pragma once

#include "entt.hpp"
#include <string>
#include <optional>
#include "sol\sol.hpp"
#include <ECS\Component\TransformComponent.h>
#include <ECS\Component\PhysicsBodyComponents\PhysicsBodyComponent.h>

class Entity
{
public:

	Entity(const std::string& name, entt::registry* registry);

	entt::entity GetEntity() const;

	template <typename ComponentType, typename... Args>
	ComponentType* CreateComponent(Args&&... args)
	{
		return &m_registry->emplace<ComponentType>(m_entity, std::forward<Args>(args) ...);
	}	
	
	template <typename ComponentType>
	std::optional<ComponentType> GetComponent()
	{
		if (HasComponent<ComponentType>())
		{
			return std::optional<ComponentType>(m_registry->get<ComponentType>(m_entity));
		}
		return std::nullopt;
	}

	template <typename ComponentType>
	bool HasComponent() 
	{
		return m_registry->all_of<ComponentType>(m_entity);
	}

	static void LUABind(sol::state& lua);

private:
	entt::entity m_entity;
	entt::registry* m_registry;

	std::string m_name;
};

