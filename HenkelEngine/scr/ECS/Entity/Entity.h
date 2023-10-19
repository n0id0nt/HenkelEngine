#pragma once

#include <vector>
#include <memory>
#include "Transform.h"
#include "ECS/Component/Component.h"
#include <string>

class Scene;

class Entity
{
public:
	Entity(Scene* scene, std::string name);

	void Update(float deltaTime);

	void SetName(std::string name);
	std::string GetName() const;

	void AddComponent(Component* component);

private:
	std::string m_name;

	std::vector<std::unique_ptr<Component>> m_components;

	std::unique_ptr<Transform> m_transform;
	Scene* m_scene;

public:
	Transform* GetTransform() const { return m_transform.get(); }
	Scene* GetScene() const { return m_scene; }

	template <class T>
	T* GetComponent() const 
	{
		for (auto& component : m_components)
		{
			T* conponentType = dynamic_cast<T*>(component.get());
			if (conponentType)
				return conponentType;
		}
		return nullptr;
	}

	template <class T>
	const bool HasComponent() const 
	{
		for (auto& component : m_components)
		{
			if (dynamic_cast<T*>(component.get()))
				return true;
		}
		return false;
	}
	 
};

