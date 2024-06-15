#pragma once

#include <string>
#include <memory>
#include "sol\sol.hpp"
#include "ECS\Registry\Registry.h"
#include <set>

class Entity
{
public:

	Entity(const std::string& name, Registry* registry);
	Entity(const Entity&) = delete;

	EntityId GetEntityId() const;

	std::string GetName() const;

	void SetChild(Entity* entity);
	void SetParent(Entity* entity);

	// schedules the deletion of the entity at the end of the current frame
	void Delete();
	bool IsMarkedForDeletion();

	void RemoveChild(Entity* entity);
	void RemoveParent();

	void AddTag(std::string tag);
	void RemoveTag(std::string tag);
	bool HasTag(std::string tag);

	std::vector<Entity*> GetChildren() const;
	Entity* GetParent() const;

	bool GetIsIndependentFromLevel() const;

	template <typename ComponentType, typename... Args>
	ComponentType* CreateComponent(Args&&... args) const
	{
		return m_registry->AddComponent<ComponentType>(m_entity, std::forward<Args>(args) ...);
	}	
	
	template <typename ComponentType>
	ComponentType* GetComponent() const
	{
		return m_registry->GetComponent<ComponentType>(m_entity);
	}

	template <typename ComponentType>
	bool HasComponent() const
	{
		return m_registry->HasComponent<ComponentType>(m_entity);
	}

	static void LUABind(sol::state& lua);

private:
	EntityId m_entity;
	Registry* m_registry;

	Entity* m_parent;
	std::vector<Entity*> m_children;
	std::set<std::string> m_tags;

	std::string m_name;
	int m_uniqueHash = 0;

	bool m_isIndependentFromLevel;
	bool m_scheduledForDeletion;
};

