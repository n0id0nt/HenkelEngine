#include "Registry.h"

Registry::Registry() : m_components(), m_currentEntityId(0u), m_entities()
{
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
			entityComponents.second.erase(entityId);
		}
	}
}
