#include "CameraComponent.h"
#include <Engine.h>
#include "imgui\imgui.h"

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
	const float inputFieldWidth = 80.f;
	ImGui::Text("Camera Component:");
	ImGui::SliderFloat("zoom", &m_zoom, 0.1f, 10.f);
	bool isActive = m_isActiveCamera;
	ImGui::Checkbox("Active Camera", &isActive);
	ImGui::Checkbox("Debug Lines", &debugLines);
	ImGui::Text("Soft Damping:");
	ImGui::InputFloat("X##UniqueID1", &m_softDamping.x);
	ImGui::InputFloat("Y##UniqueID1", &m_softDamping.y);	
	ImGui::Text("Hard Damping:");
	ImGui::InputFloat("X##UniqueID2", &m_hardDamping.x);
	ImGui::InputFloat("Y##UniqueID2", &m_hardDamping.y);	
	ImGui::Text("Camera Offset:");
	ImGui::SliderFloat("X##UniqueID3", &m_offset.x, -1.f, 1.f);
	ImGui::SliderFloat("Y##UniqueID3", &m_offset.y, -1.f, 1.f);
	ImGui::Text("Soft Zone:");
	ImGui::SliderFloat("X##UniqueID5", &m_softZone.x, 0.f, 1.f);
	ImGui::SliderFloat("Y##UniqueID5", &m_softZone.y, 0.f, 1.f);
	ImGui::Text("Dead Zone:");
	ImGui::SliderFloat("X##UniqueID4", &m_deadZone.x, 0.f, m_softZone.x);
	ImGui::SliderFloat("Y##UniqueID4", &m_deadZone.y, 0.f, m_softZone.y);

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
