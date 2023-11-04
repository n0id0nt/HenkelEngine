#include "Entity.h"
#include "ECS\Component\TransformComponent.h"
#include "ECS\Component\PhysicsBodyComponents\PhysicsBodyComponent.h"

Entity::Entity(const std::string& name, Registry* registry)
	: m_name(name), m_registry(registry), m_parent(nullptr), m_children()
{
	m_entity = m_registry->CreateEntity();
}

EntityId Entity::GetEntity() const
{
	return m_entity;
}

void Entity::SetChild(Entity* entity)
{
	if (entity->m_parent == this)
		return;

	entity->RemoveParent(entity->m_parent);

	m_children.push_back(entity);
	entity->m_parent = this;
}

void Entity::SetParent(Entity* entity)
{
	if (m_parent == this)
		return;

	if (m_parent)
		m_parent->RemoveChild(this);

	m_parent = entity;
	m_parent->m_children.push_back(entity);
}

void Entity::RemoveChild(Entity* entity)
{
	if (entity->m_parent != this)
		return;

	entity->m_parent = nullptr;
	m_children.erase(std::remove(m_children.begin(), m_children.end(), entity), m_children.end());
}

void Entity::RemoveParent(Entity* entity)
{
	if (m_parent != entity)
		return;

	m_parent = nullptr;
	entity->m_children.erase(std::remove(m_children.begin(), m_children.end(), entity), m_children.end());
}

std::vector<Entity*> Entity::GetChildren() const
{
	return m_children;
}

Entity* Entity::GetParent() const
{
	return m_parent;
}

void Entity::LUABind(sol::state& lua)
{
	lua.new_usertype<Entity>("GO",
		"getTransform", &Entity::GetComponent<TransformComponent>,
		"getPhysicsBody", &Entity::GetComponent<PhysicsBodyComponent>
	);
}
