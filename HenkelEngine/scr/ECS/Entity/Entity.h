#pragma once

#include <string>
#include <memory>
#include "sol\sol.hpp"
#include "ECS\Registry.h"

class Entity
{
public:

	Entity(const std::string& name, Registry* registry);
	Entity(const Entity&) = delete;

	EntityId GetEntityId() const;

	std::string GetName() const;

	void SetChild(Entity* entity);
	void SetParent(Entity* entity);

	void RemoveChild(Entity* entity);
	void RemoveParent(Entity* entity);

	std::vector<Entity*> GetChildren() const;
	Entity* GetParent() const;

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

	Entity* m_parent;
	std::vector<Entity*> m_children;

	std::string m_name;
};

