#include "CameraComponent.h"
#include <Engine.h>
#include "imgui.h"
#include "DebugGUIPanels/ImGuiHelper.h"

CameraComponent::CameraComponent() 
	: m_zoom(1.f), m_angle(0.f), m_isActiveCamera(false), debugLines(false), m_offset(glm::vec2()), 
	m_deadZone(glm::vec2()), m_damping(glm::vec2(1.f,1.f)), m_forcePosition(false),
	m_trauma(0.f), m_traumaTime(1.f), m_maxOffset(4.f), m_maxAngle(3.f), m_shakeAmplitude(67.8f)
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
	MakeCameraActiveFromCamera(Engine::GetInstance()->GetCurrentScene()->GetCamera());
}

void CameraComponent::MakeCameraActiveFromCamera(Camera* camera)
{
	if (m_isActiveCamera)
		return;
	camera->SetActiveCameraComponent(this);
	m_isActiveCamera = true;
}

void CameraComponent::MakeCameraInactive()
{
	MakeCameraInactiveFromCamera(Engine::GetInstance()->GetCurrentScene()->GetCamera());
}

void CameraComponent::MakeCameraInactiveFromCamera(Camera* camera)
{
	if (!m_isActiveCamera)
		return;
	m_isActiveCamera = false;
	camera->SetActiveCameraComponent(nullptr);
}

void CameraComponent::DrawDebugPanel()
{
	ImGui::SliderFloat("Zoom", &m_zoom, 0.1f, 10.f);
	ImGui::SliderFloat("Angle", &m_angle, -180.f, 180.f);
	bool isActive = m_isActiveCamera;
	ImGui::Checkbox("Active Camera", &isActive);
	ImGui::SameLine();
	ImGui::Checkbox("Debug Lines", &debugLines);
	ImGui::SameLine();
	if (ImGui::Button("Force Position"))
	{
		SetForcePosition(true);
	}

	ImGui::DrawVec2Control("Damping", m_damping);
	ImGui::DrawVec2Control("Camera Offset", m_offset);
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

	static float debugTraumaAmount = 0.2f;
	if (ImGui::Button("Add Trauma"))
	{
		AddTrauma(debugTraumaAmount);
	}
	ImGui::SameLine();
	ImGui::SliderFloat("Trauma Amount", &debugTraumaAmount, 0.f, 1.f);
	ImGui::DragFloat("Max Shake Angle", &m_maxAngle);
	ImGui::DragFloat("Max Shake Offset", &m_maxOffset);
	ImGui::DragFloat("Trauma Time", &m_traumaTime);
	ImGui::DragFloat("Shake Amplitude", &m_shakeAmplitude);
	ImGui::SliderFloat("Trauma Value", &m_trauma, 0.f, 1.f);
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

void CameraComponent::SetDamping(glm::vec2 damping)
{
	m_damping = damping;
}

glm::vec2 CameraComponent::GetDamping()
{
	return m_damping;
}

void CameraComponent::SetAngle(float angle)
{
	m_angle = angle;
}

float CameraComponent::GetAngle()
{
	return m_angle;
}

void CameraComponent::ForcePosition()
{
	m_forcePosition = true;
}

void CameraComponent::SetForcePosition(bool value)
{
	m_forcePosition = value;
}

bool CameraComponent::IsPositionForced()
{
	return m_forcePosition;
}

void CameraComponent::AddTrauma(float trauma)
{
	m_trauma += trauma;
	if (m_trauma > 1.f) m_trauma = 1.f;
}

float CameraComponent::GetTrauma()
{
	return m_trauma;
}

void CameraComponent::SetTrauma(float trauma)
{
	m_trauma = trauma;
}

void CameraComponent::ResetTrauma()
{
	m_trauma = 0.f;
}

float CameraComponent::GetTraumaTime()
{
	return m_traumaTime;
}

float CameraComponent::GetMaxOffset()
{
	return m_maxOffset;
}

float CameraComponent::GetMaxAngle()
{
	return m_maxAngle;
}

float CameraComponent::GetShakeAmplitude()
{
	return m_shakeAmplitude;
}

void CameraComponent::LUABind(sol::state& lua)
{
	lua.new_usertype<CameraComponent>("Camera",
		"setZoom", &CameraComponent::SetZoom,
		"getZoom", &CameraComponent::GetZoom,
		"isActiveCamera", &CameraComponent::IsActiveCamera,
		"makeCameraActive", &CameraComponent::MakeCameraActive,
		"makeCameraInactive", &CameraComponent::MakeCameraInactive,
		"setOffset", &CameraComponent::SetOffset,
		"getOffset", &CameraComponent::GetOffset,
		"setDeadZone", &CameraComponent::SetDeadZone,
		"getDeadZone", &CameraComponent::GetDeadZone,
		"setDamping", &CameraComponent::SetDamping,
		"getDamping", &CameraComponent::GetDamping,
		"forcePosition", &CameraComponent::ForcePosition,
		"addShakeTrauma", &CameraComponent::AddTrauma
	);
}

