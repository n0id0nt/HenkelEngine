#include "CameraComponent.h"
#include <Engine.h>
#include "imgui\imgui.h"

CameraComponent::CameraComponent() : m_zoom(1.f), m_isActiveCamera(false)
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
