#include "Registry.h"

Registry::Registry() : m_components(), m_currentEntityId(0u), m_entities()
{
}

Registry::~Registry()
{
	for (auto& entityComponents : m_components)
	{
		for (auto& component : entityComponents.second)
		{
			std::free(component.second);
		}
		entityComponents.second.clear();
	}
	m_components.clear();
	m_entities.clear();
}

EntityId Registry::CreateEntity()
{
	EntityId newEntityId = m_currentEntityId;
	m_currentEntityId++;
	m_entities.push_back(newEntityId);
	return newEntityId;
}

void Registry::DeleteEntity(EntityId entityId)
{
	m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), entityId), m_entities.end());

	for (auto& entityComponents : m_components)
	{
		auto component = entityComponents.second.find(entityId);

		if (component != entityComponents.second.end())
		{
			std::free(component->second);
			entityComponents.second.erase(entityId);
		}
	}
}
