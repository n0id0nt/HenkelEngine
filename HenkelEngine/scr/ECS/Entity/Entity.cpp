#include "Entity.h"
#include "ECS\Component\TransformComponent.h"
#include "ECS\Component\PhysicsBodyComponents\PhysicsBodyComponent.h"
#include "ECS\Component\PhysicsBodyComponents\StaticBodyComponent.h"
#include "ECS\Component\PhysicsBodyComponents\TileMapCollisionBodyComponent.h"
#include "ECS/Component/RenderComponents/SpriteComponent.h"
#include "ECS/Component/RenderComponents/RenderComponent.h"
#include <ECS\Component\SpriteAnimationComponent.h>
#include <ECS\Component\CameraComponent.h>
#include <ECS\Component\UIComponent.h>

Entity::Entity(const std::string& name, Registry* registry)
	: m_name(name), m_registry(registry), m_parent(nullptr), m_children(), m_tags(), m_isIndependentFromLevel(false)
{
	m_entity = m_registry->CreateEntity();
}

EntityId Entity::GetEntityId() const
{
	return m_entity;
}

std::string Entity::GetName() const
{
	return m_name;
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
	m_parent->m_children.push_back(this);
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

void Entity::AddTag(std::string tag)
{
	m_tags.insert(tag);
}

void Entity::RemoveTag(std::string tag)
{
	m_tags.erase(tag);
}

bool Entity::HasTag(std::string tag)
{
	return m_tags.contains(tag);
}

std::vector<Entity*> Entity::GetChildren() const
{
	return m_children;
}

Entity* Entity::GetParent() const
{
	return m_parent;
}

bool Entity::GetIsIndependentFromLevel() const
{
	return m_isIndependentFromLevel;
}

void Entity::LUABind(sol::state& lua)
{
	lua.new_usertype<Entity>("GO",
		"getName", &Entity::GetName,
		"getTransform", &Entity::GetComponent<TransformComponent>,
		"getSprite", &Entity::GetComponent<SpriteComponent>,
		"getSpriteAnimation", &Entity::GetComponent<SpriteAnimationComponent>,
		"getPhysicsBody", &Entity::GetComponent<PhysicsBodyComponent>,
		"getStaticBody", &Entity::GetComponent<StaticBodyComponent>,
		"getTileMapCollisionBodyComponent", &Entity::GetComponent<TileMapCollisionBodyComponent>,
		"getUILayout", &Entity::GetComponent<UIComponent>,
		"getCamera", &Entity::GetComponent<CameraComponent>,
		"getRenderer", &Entity::GetComponent<RenderComponent>,
		"addTag", &Entity::AddTag,
		"removeTag", &Entity::RemoveTag,
		"hasTag", &Entity::HasTag
	);
}
