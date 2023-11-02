#include "Entity.h"

Entity::Entity(const std::string& name, Registry* registry) 
	: m_name(name), m_registry(registry)
{
	m_entity = m_registry->CreateEntity();
}

EntityId Entity::GetEntity() const
{
	return m_entity;
}

void Entity::LUABind(sol::state& lua)
{
	lua.new_usertype<Entity>("GO",
		"getTransform", &Entity::GetComponent<TransformComponent>,
		"getPhysicsBody", &Entity::GetComponent<PhysicsBodyComponent>
	);
}
