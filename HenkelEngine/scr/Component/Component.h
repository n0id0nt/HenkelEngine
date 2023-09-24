#pragma once

class Entity;

class Component
{
public:
	Component(Entity* entity) : p_entity(entity) {}

	virtual void Update() = 0;

	Entity* GetEntity() const { return p_entity; }

private:
	Entity* p_entity;
};

