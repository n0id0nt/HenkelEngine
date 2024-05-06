#pragma once

#include <map>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <memory>
#include <any>
#include "opengl\openglHelper.h"

typedef unsigned int EntityId;

class Registry
{
public:
	Registry();
    ~Registry();

    EntityId CreateEntity();
    void DeleteEntity(EntityId entityId);

    // Add a component to an entity
    template <typename ComponentType, typename... Args>
    ComponentType* AddComponent(EntityId entityId, Args&&... args)
    {
        void* componentRawPtr = std::malloc(sizeof(ComponentType));
        componentRawPtr = new ComponentType(std::forward<Args>(args)...);
        m_components[std::type_index(typeid(ComponentType))][entityId] = componentRawPtr;
        return static_cast<ComponentType*>(componentRawPtr);
    }

    // Get a component from an entity
    template <typename ComponentType>
    ComponentType* GetComponent(EntityId entityId)
    {
        auto typeIndex = std::type_index(typeid(ComponentType));
        auto entityComponents = m_components.find(typeIndex);

        if (entityComponents != m_components.end()) 
        {
            auto componentIter = entityComponents->second.find(entityId);
            if (componentIter != entityComponents->second.end()) 
            {
                ComponentType* component = static_cast<ComponentType*>(componentIter->second);
                return component;
            }
        }
        //ASSERT(false); // No component of type found
        return nullptr;
    }

    // Remove a component from an entity
    template <typename ComponentType>
    void RemoveComponent(EntityId entityId)
    {
        auto typeIndex = std::type_index(typeid(ComponentType));
        auto entityComponents = m_components.find(typeIndex);

        if (entityComponents != m_components.end()) 
        {
            std::free(entityComponents->second);
            entityComponents->second.erase(entityId);
        }
    }

    // Check if an entity has a specific component
    template <typename ComponentType>
    bool HasComponent(EntityId entityId)
    {
        auto typeIndex = std::type_index(typeid(ComponentType));
        auto entityComponents = m_components.find(typeIndex);

        if (entityComponents != m_components.end()) 
        {
            return entityComponents->second.find(entityId) != entityComponents->second.end();
        }

        return false;
    }

    // Retrieve all entities with a specific component type
    template <typename ComponentType>
    std::vector<EntityId> GetEntitiesWithComponent()
    {
        std::vector<EntityId> entitiesWithComponent;

        auto typeIndex = std::type_index(typeid(ComponentType));
        auto entityComponents = m_components.find(typeIndex);

        if (entityComponents != m_components.end()) 
        {
            entitiesWithComponent.reserve(entityComponents->second.size());
            for (const auto& pair : entityComponents->second) 
            {
                entitiesWithComponent.push_back(pair.first);
            }
        }

        return entitiesWithComponent;
    }

    // Retrieve all entities with multiple matching component types
    template <typename... ComponentTypes>
    std::vector<EntityId> GetEntitiesWithComponents()
    {
        std::vector<EntityId> matchingEntities; 

        // TODO Get get all entities with the first component and check all the entities in the list share the same component
        for (auto& entityId : m_entities)
        {
			bool hasAllComponents = (HasComponent<ComponentTypes>(entityId) && ...);

			//bool hasAllComponents = HasAllComponents<ComponentTypes>(entityComponents.second);
			if (hasAllComponents)
			{
		        matchingEntities.push_back(entityId);
			}
        }

        return matchingEntities;
    }
    
    // Retrieve all entities with multiple matching component types
    template <typename ComponentType>
    std::vector<ComponentType*> GetAllComponents()
    {
        std::vector<ComponentType*> components;

        auto typeIndex = std::type_index(typeid(ComponentType));
        auto entityComponents = m_components.find(typeIndex);

        if (entityComponents != m_components.end())
        {
            components.reserve(entityComponents->second.size());
            for (auto& entityComponent : entityComponents->second)
            {
                ComponentType* component = static_cast<ComponentType*>(entityComponent.second);
                components.push_back(component);
            }
        }
        return components;
    }


private:

    EntityId m_currentEntityId;

	std::unordered_map<std::type_index, std::unordered_map<EntityId, void*>> m_components;
	std::vector<EntityId> m_entities;
};

