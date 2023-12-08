#include "CameraComponent.h"
#include <Engine.h>
#include "imgui.h"
#include "DebugGUIPanels/ImGuiHelper.h"

CameraComponent::CameraComponent() 
	: m_zoom(1.f), m_isActiveCamera(false), debugLines(false), m_offset(glm::vec2()), 
	m_deadZone(glm::vec2()), m_softZone(glm::vec2(0.5f, 0.5f)), m_softDamping(glm::vec2()), m_hardDamping(glm::vec2())
{
}

void CameraComponent::SetZoom(float zoom)
{
	m_zoom = zoom;
}

float CameraComponent::GetZoom() const
{
	return m_zoom;
}

bool CameraComponent::IsActiveCamera() const
{
	return m_isActiveCamera;
}

void CameraComponent::MakeCameraActive()
{
	if (m_isActiveCamera) 
		return;
	Engine::GetInstance()->GetCurrentScene()->GetCamera()->SetActiveCameraComponent(this);
	m_isActiveCamera = true;
}

void CameraComponent::MakeCameraInactive()
{
	if (!m_isActiveCamera)
		return;
	m_isActiveCamera = false;
	Engine::GetInstance()->GetCurrentScene()->GetCamera()->SetActiveCameraComponent(nullptr);
}

void CameraComponent::DrawDebugPanel()
{
	ImGui::SliderFloat("Zoom", &m_zoom, 0.1f, 10.f);
	bool isActive = m_isActiveCamera;
	ImGui::Checkbox("Active Camera", &isActive);
	ImGui::SameLine();
	ImGui::Checkbox("Debug Lines", &debugLines);

	ImGui::DrawVec2Control("Soft Damping", m_softDamping);
	ImGui::DrawVec2Control("Hard Damping", m_hardDamping);
	ImGui::DrawVec2Control("Camera Offset", m_offset);
	ImGui::DrawVec2Control("Soft Zone", m_softZone);
	ImGui::DrawVec2Control("Dead Zone", m_deadZone);

	if (isActive != m_isActiveCamera)
	{
		if (isActive)
		{
			MakeCameraActive();
		}
		else
		{
			MakeCameraInactive();
		}
	}
}

void CameraComponent::SetOffset(glm::vec2 offset)
{
	m_offset = offset;
}

glm::vec2 CameraComponent::GetOffset()
{
	return m_offset;
}

void CameraComponent::SetDeadZone(glm::vec2 deadZone)
{
	m_deadZone = deadZone;
}

glm::vec2 CameraComponent::GetDeadZone()
{
	return m_deadZone;
}

void CameraComponent::SetSoftZone(glm::vec2 softZone)
{
	m_softZone = softZone;
}

glm::vec2 CameraComponent::GetSoftZone()
{
	return m_softZone;
}

void CameraComponent::SetSoftDamping(glm::vec2 softDamping)
{
	m_softDamping = softDamping;
}

glm::vec2 CameraComponent::GetSoftDamping()
{
	return m_softDamping;
}

void CameraComponent::SetHardDamping(glm::vec2 hardDamping)
{
	m_hardDamping = hardDamping;
}

glm::vec2 CameraComponent::GetHardDamping()
{
	return m_hardDamping;
}
