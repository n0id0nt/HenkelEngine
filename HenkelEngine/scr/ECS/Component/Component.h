#pragma once

class Entity;

class Component
{
public:
	Component(Entity* entity) : m_entity(entity) {}

	virtual void Update(float deltaTime) = 0;

	Entity* GetEntity() const { return m_entity; }

private:
	Entity* m_entity;
};

