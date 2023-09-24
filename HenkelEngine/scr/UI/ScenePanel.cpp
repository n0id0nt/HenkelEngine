#include "ScenePanel.h"
#include "../Scene.h"
#include "..\Component\RendererComponent.h"
#include <iostream>


void ScenePanel::Render()
{
	StartPanelRender();

	m_frameBuffer->Bind();

	for (auto& entity : m_scene->GetEntities())
	{
		if (entity->HasComponent<RendererComponent>())
		{
			entity->GetComponent<RendererComponent>()->Render(this);
		}
	}

	m_frameBuffer->Unbind();


	uint64_t textureID = m_frameBuffer->GetTexture();
	ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_size.x, m_size.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

	EndPanelRender();
}

void ScenePanel::OnResize()
{
	m_frameBuffer->CreateBuffer(m_size.x, m_size.y);
	//std::cout << "Resize " << m_size.x << ", " << m_size.y << std::endl;
}
