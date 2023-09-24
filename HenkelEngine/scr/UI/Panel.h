#pragma once

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <imgui_internal.h>
#include <string>

class Scene;

class Panel
{
public:
	Panel(std::string name, Scene* scene)
		: m_name(name), m_size(), m_scene(scene), m_docID(), visible(true)
	{}

	void SetParentNode(ImGuiID nodeID)
	{
		m_docID = nodeID;
	}

	virtual void Render() = 0;
	
	ImVec2 GetSize() const { return m_size; }
	std::string GetName() const { return m_name; }

	ImGuiID GetDocID() const { return m_docID; }

	bool IsVisible() const { return visible; }

	virtual void OnResize() = 0;

protected:

	void StartPanelRender()
	{
		ImGui::Begin(m_name.c_str(), &visible);

		ImVec2 newSize = ImGui::GetContentRegionAvail();
		if (m_size.x != newSize.x || m_size.y != newSize.y)
		{
			m_size = newSize;
			OnResize();
		}
	}

	void EndPanelRender()
	{
		ImGui::End();
	}

	ImGuiID m_docID;
	ImVec2 m_size;
	std::string m_name;
	Scene* m_scene;

	bool visible;
};

