#include "Scene.h"
#include "Component\RendererComponent.h"
#include "Window.h"
#include <imgui_internal.h>
#include "UI\MovePanel.h"
#include "UI\ScenePanel.h"

Scene::Scene(Window* window)
	: m_entities(), m_camera(new Camera(glm::vec3(0.0f, 0.0f, 6.0f))), m_window(window), m_panels(), m_dockingEnviromentInited(false)
{
	// create the rubiks cube and respective components
	//Entity* rubiksCube = CreateEntity("Rubiks Cube");
	//rubiksCube->AddComponent(new RendererComponent(entity, glm::vec3(0.1f, 0.9f, 0.1f)));

}

Scene::~Scene()
{
	
}

void Scene::Update()
{
	for (auto& entity : m_entities)
	{
		entity->Update();
	}
}

void Scene::Render()
{
	CreateDockingEnviroment();

	// draw all the panels
	for (auto& panel : m_panels)
	{
		panel->Render();
	}
}

Entity* Scene::CreateEntity(std::string name)
{
	m_entities.push_back(std::make_unique<Entity>(this, name));
	return m_entities.back().get();
}

void Scene::RemoveEntity(Entity* entity)
{
}

void Scene::CreateDockingEnviroment()
{
	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_NoTabBar;

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		windowFlags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("InvisibleWindow", nullptr, windowFlags);
	ImGui::PopStyleVar();
	ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspaceID = ImGui::GetID("InvisibleWindowDockSpace");
		ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockspace_flags);

		if (!m_dockingEnviromentInited)
		{
			m_dockingEnviromentInited = true;

			InitDockingEnviroment(dockspaceID, dockspace_flags);
		}
	}
	ImGui::End();
}

void Scene::InitDockingEnviroment(ImGuiID dockspaceID, ImGuiDockNodeFlags dockspace_flags)
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::DockBuilderRemoveNode(dockspaceID); // clear any previous layout
	ImGui::DockBuilderAddNode(dockspaceID, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
	ImGui::DockBuilderSetNodeSize(dockspaceID, viewport->Size);

	CreatePanels(dockspaceID);

	// we now dock our windows into the docking node we made above
	for (auto& panel : m_panels)
	{
		ImGui::DockBuilderDockWindow(panel->GetName().c_str(), panel->GetDocID());
	}

	ImGui::DockBuilderFinish(dockspaceID);
}

void Scene::CreatePanels(ImGuiID dockspaceID)
{
	auto dock_id_right = ImGui::DockBuilderSplitNode(dockspaceID, ImGuiDir_Right, 0.3f, nullptr, &dockspaceID);

	m_panels.push_back(std::make_unique<MovePanel>("Moves", this));
	m_panels[0]->SetParentNode(dock_id_right);

	m_panels.push_back(std::make_unique<ScenePanel>("Scene", this));
	m_panels[1]->SetParentNode(dockspaceID);
}
