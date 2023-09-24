#include "Entity.h"
#include "../Scene.h"

Entity::Entity(Scene* scene, std::string name) 
	: m_scene(scene), m_name(name), m_components(), m_transform(std::make_unique<Transform>())
{

}

void Entity::Update()
{
	for (auto& component : m_components)
	{
		component.get()->Update();
	}
}

void Entity::SetName(std::string name)
{
	m_name = name;
}

std::string Entity::GetName() const
{
	return m_name;
}

void Entity::AddComponent(Component* component)
{
	m_components.push_back(std::unique_ptr<Component>(component));
}
