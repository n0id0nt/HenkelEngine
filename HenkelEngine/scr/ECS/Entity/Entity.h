#pragma once

#include <string>
#include "sol\sol.hpp"
#include "ECS\Registry.h"
#include "ECS\Component\TransformComponent.h"
#include "ECS\Component\PhysicsBodyComponents\PhysicsBodyComponent.h"

class Entity
{
public:

	Entity(const std::string& name, Registry* registry);

	EntityId GetEntity() const;

	template <typename ComponentType, typename... Args>
	ComponentType* CreateComponent(Args&&... args)
	{
		return m_registry->AddComponent<ComponentType>(m_entity, std::forward<Args>(args) ...);
	}	
	
	template <typename ComponentType>
	ComponentType* GetComponent()
	{
		return m_registry->GetComponent<ComponentType>(m_entity);
	}

	template <typename ComponentType>
	bool HasComponent() 
	{
		return m_registry->HasComponent<ComponentType>(m_entity);
	}

	static void LUABind(sol::state& lua);

private:
	EntityId m_entity;
	Registry* m_registry;

	std::string m_name;
};

