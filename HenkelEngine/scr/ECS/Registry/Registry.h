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

class IComponentArray
{
public:
    virtual ~IComponentArray() = default;
    virtual void EntityDestroyed(EntityId entityId) = 0;
};

template<typename ComponentType>
class ComponentArray : public IComponentArray
{
public:
    template <typename... Args>
    ComponentType* AddComponent(EntityId entityId, Args&&... args)
    {
        auto* component = new ComponentType(std::forward<Args>(args)...);
        m_components[entityId] = component;
        return component;
    }

    bool HasComponent(EntityId entityId)
    {
        auto componentArrayIter = m_components.find(entityId);
        return componentArrayIter != m_components.end();
    }

    ComponentType* GetComponent(EntityId entityId)
    {
        auto componentArrayIter = m_components.find(entityId);
        if (componentArrayIter != m_components.end())
        {
            return componentArrayIter->second;
        }
        return nullptr;
    }

    void RemoveComponent(EntityId entityId)
    {
        delete m_components[entityId];
        m_components.erase(entityId);
    }

    void EntityDestroyed(EntityId entityId) override
    {
        if (m_components.find(entityId) != m_components.end())
        {
            // Remove the entity's component if it existed
            RemoveComponent(entityId);
        }
    }

    size_t Size()
    {
        return m_components.size();
    }

    //std::vector<ComponentType> m_componentArray;
    std::unordered_map<EntityId, ComponentType*> m_components;
    //std::unordered_map<size_t, EntityId>
};

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
        std::type_index typeIndex(typeid(ComponentType));
        auto componentArrayIter = m_components.find(typeIndex);
        if (componentArrayIter == m_components.end())
        {
            ComponentArray<ComponentType>* componentRawPtr = new ComponentArray<ComponentType>();
            m_components[typeIndex] = componentRawPtr;
            auto component = componentRawPtr->AddComponent(entityId, std::forward<Args>(args)...);
            return component;
        }
        ComponentArray<ComponentType>* componentArray = dynamic_cast<ComponentArray<ComponentType>*>(componentArrayIter->second);
        auto component = componentArray->AddComponent(entityId, std::forward<Args>(args)...);
        return component;
    }

    // Get a component from an entity
    template <typename ComponentType>
    ComponentType* GetComponent(EntityId entityId)
    {
        std::type_index typeIndex(typeid(ComponentType));
        auto componentArrayIter = m_components.find(typeIndex);
        if (componentArrayIter != m_components.end())
        {
            ComponentArray<ComponentType>* componentArray = dynamic_cast<ComponentArray<ComponentType>*>(componentArrayIter->second);
            ComponentType* component = componentArray->GetComponent(entityId);
            return component;
        }
        return nullptr;
    }

    // Remove a component from an entity
    template <typename ComponentType>
    void RemoveComponent(EntityId entityId)
    {
        std::type_index typeIndex(typeid(ComponentType));
        auto componentArrayIter = m_components.find(typeIndex);
        ASSERT(componentArrayIter != m_components.end())
        {
            ComponentArray<ComponentType>* componentArray = dynamic_cast<ComponentArray<ComponentType>*>(componentArrayIter->second);
            ComponentType* component = componentArray->RemoveComponent(entityId);
        }
    } 

    // Check if an entity has a specific component
    template <typename ComponentType>
    bool HasComponent(EntityId entityId)
    {
        std::type_index typeIndex(typeid(ComponentType));
        auto componentArrayIter = m_components.find(typeIndex);
        if (componentArrayIter != m_components.end())
        {
            ComponentArray<ComponentType>* componentArray = dynamic_cast<ComponentArray<ComponentType>*>(componentArrayIter->second);
            return componentArray->HasComponent(entityId);
        }
        return false;
    }

    // Retrieve all entities with a specific component type
    template <typename ComponentType>
    std::vector<EntityId> GetEntitiesWithComponent()
    {
        std::vector<EntityId> entitiesWithComponent;
        std::type_index typeIndex(typeid(ComponentType));
        auto componentArrayIter = m_components.find(typeIndex);
        if (componentArrayIter != m_components.end())
        {
            ComponentArray<ComponentType>* componentArray = dynamic_cast<ComponentArray<ComponentType>*>(componentArrayIter->second);
            entitiesWithComponent.reserve(componentArray->m_components.size());
            for (auto& pair : componentArray->m_components)
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
			if (hasAllComponents)
			{
		        matchingEntities.push_back(entityId);
			}
        }
        return matchingEntities;
    }
    
    // Retrieve all components of type
    template <typename ComponentType>
    std::vector<ComponentType*> GetAllComponents()
    {
        std::vector<ComponentType*> components;
        std::type_index typeIndex(typeid(ComponentType));
        auto componentArrayIter = m_components.find(typeIndex);
        if (componentArrayIter != m_components.end())
        {
            ComponentArray<ComponentType>* componentArray = dynamic_cast<ComponentArray<ComponentType>*>(componentArrayIter->second);
            components.reserve(componentArray->m_components.size());
            for (auto& pair : componentArray->m_components)
            {
                ComponentType* component = pair.second;
                components.push_back(component);
            }
        }
        return components;
    }


private:

    EntityId m_currentEntityId;

	std::unordered_map<std::type_index, IComponentArray*> m_components;
	std::vector<EntityId> m_entities;
};

