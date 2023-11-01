#include "Registry.h"

Registry::Registry() : m_components(), m_currentEntityId(0u)
{
}

EntityId Registry::CreateEntity()
{
	EntityId newEntityId = m_currentEntityId;
	m_currentEntityId++;
	return newEntityId;
}
