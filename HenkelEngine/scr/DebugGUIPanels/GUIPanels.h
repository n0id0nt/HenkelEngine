#pragma once

#include "imgui.h"
#include <string>
#include <format>
#include "ECS/Component/TransformComponent.h"
#include "ECS/Component/ScriptComponent.h"
#include "ECS/Component/PhysicsBodyComponents/StaticBodyComponent.h"
#include "ECS/Component/PhysicsBodyComponents/PhysicsBodyComponent.h"
#include "ECS/Component/PhysicsBodyComponents/TileMapCollisionBodyComponent.h"

namespace GUIPanel
{
	const float s_indentAmount = 20.f;
	static bool s_isComponetWindowOpen = false;
	static Entity* s_focusedComponet = nullptr;

	class EntityHierarchy
	{
	public:
		static void Panel(const std::vector<std::unique_ptr<Entity>>& entities)
		{
			ImGui::Begin("Enity Hierarchy");
			for (auto& entity : entities)
			{
				if (entity->GetParent())
					continue;

				RenderTreeNode(entity.get());
			}
			ImGui::End();

			if (s_isComponetWindowOpen) 
			{
				ImGui::Begin("Component List", &s_isComponetWindowOpen);
				DrawComponentList(s_focusedComponet);
				ImGui::End();
			}
		}

	private:

		static void DrawComponentList(Entity* entity)
		{
			ImGui::Text(std::format("Id: {},\tName: {}", entity->GetEntity(), entity->GetName()).c_str());
			auto* transform = entity->GetComponent<TransformComponent>();
			auto* staticBody = entity->GetComponent<StaticBodyComponent>();
			auto* physicsBody = entity->GetComponent<PhysicsBodyComponent>();
			auto* tileMapCollisionBody = entity->GetComponent<TileMapCollisionBodyComponent>();
			auto* script = entity->GetComponent<ScriptComponent>();
			if (transform)
			{
				transform->DrawDebugPanel();
				ImGui::SameLine();
			}
		}

		static void RenderTreeNode(Entity* entity)
		{
			bool isNodeOpen = ImGui::CollapsingHeader(std::format("Id: {},\tName: {}", entity->GetEntity(), entity->GetName()).c_str());
			if (isNodeOpen) {
				ImGui::Indent(s_indentAmount);
				if (ImGui::Button(std::format("View Component List##UniqueID{}", entity->GetEntity()).c_str())) {
					s_isComponetWindowOpen = true;
					s_focusedComponet = entity;
				}
				for (auto& child : entity->GetChildren())
				{
					RenderTreeNode(child);
				}
				ImGui::Unindent(s_indentAmount);
			}
		}
	};
}