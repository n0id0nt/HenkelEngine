#pragma once

#include <map>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <memory>

typedef unsigned int EntityId;

class Registry
{
public:
	Registry();

    EntityId CreateEntity();

    // Add a component to an entity
    template <typename ComponentType, typename... Args>
    ComponentType* AddComponent(EntityId entityId, Args... args)
    {
        std::unique_ptr<ComponentType> component = std::make_unique<ComponentType>(args...);
        ComponentType* componentRawPtr = component.get();
        m_components[std::type_index(typeid(ComponentType))][entityId] = component.get();
        return componentRawPtr;
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
                return static_cast<ComponentType*>(componentIter->second);
            }
        }

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







		//// Check for each entity if it has all specified component types
		//for (const auto& entityComponents : m_components) 
		//{
		//    //bool hasAllComponents = (HasAllComponents<ComponentTypes>(entityComponents.first) && ...);
		//    // uses fold expression???
		//
		//    bool hasAllComponents = HasAllComponents<ComponentTypes>(entityComponents.second);
		//    if (hasAllComponents) 
		//    {
		//        auto v = entityComponents.second
		//        matchingEntities.push_back(v);
		//    }
		//}

        return matchingEntities;
    }


private:

    EntityId m_currentEntityId;

	std::unordered_map<std::type_index, std::unordered_map<EntityId, void*>> m_components;





    // Helper function to check if an entity has all specified component types
    template <typename ComponentTypes, typename... Rest>
    bool HasAllComponents(const std::type_index& entityIndex) 
    {
        auto componentIter = m_components.find(entityIndex);

        return componentIter != m_components.end() &&
            componentIter->second.find(std::type_index(typeid(ComponentTypes))) != componentIter->second.end() &&
            HasAllComponents<Rest...>(entityIndex);
    }

    // Base case for recursive template function
    bool HasAllComponents() { return true; }
};

