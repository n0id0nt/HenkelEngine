#include "Camera.h"
#include <Engine.h>

Camera::Camera(glm::vec3 position, glm::vec3 up)
	: m_Postition(position), m_Up(up), m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_Fov(45.0f), m_Orthographic(true)
{
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ)
	: m_Postition(glm::vec3(posX, posY, posZ)), m_Up(glm::vec3(upX, upY, upZ)), m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_Fov(45.0f), m_Orthographic(true)
{
}

glm::mat4 Camera::GetViewMatrix()
{
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(m_angle), glm::vec3(0.f, 0.f, 1.f));
	return rotationMatrix * glm::lookAt(m_Postition, m_Postition + m_Front, m_Up);
}

float Camera::GetFov()
{
	return m_Fov;
}

void Camera::SetOrthographic(bool value)
{
	m_Orthographic = value;
}

bool Camera::GetOrthographic()
{
	return m_Orthographic;
}

void Camera::SetZoom(float zoom)
{
	m_zoom = zoom;
}

float Camera::GetZoom()
{
	return m_zoom;
}

void Camera::SetActiveCameraComponent(CameraComponent* camera)
{
	if (m_activeCameraComponent)
	{
		m_activeCameraComponent->MakeCameraInactive();
	}
	m_activeCameraComponent = camera;
}

CameraComponent* Camera::GetActiveCameraComponent()
{
	return m_activeCameraComponent;
}

void Camera::SetPosition(glm::vec3 position)
{
	m_Postition = position;
}

glm::vec3 Camera::GetPosition()
{
	return m_Postition;
}

void Camera::SetAngle(float angle)
{
	m_angle = angle;
}

float Camera::GetAngle()
{
	return m_angle;
}

glm::mat4 Camera::CalculateProjection(float width, float height)
{
	if (m_Orthographic)
	{
		float left = -width / (2.f * m_zoom);
		float right = width / (2.f * m_zoom);
		float top = height / (2.f * m_zoom);
		float bottom = -height / (2.f * m_zoom);
		glm::mat4 orthoMatrix = glm::ortho(left, right, top, bottom);
		return orthoMatrix;
	}
	else
	{
		return glm::perspective(m_Fov, width / height, m_Near, m_Far);
	}
}

glm::vec2 Camera::ScreenPosToWorldPos(glm::vec2 screenPosition)
{
	glm::mat4 view = GetViewMatrix();
	glm::mat4 proj = CalculateProjection((float)Engine::GetInstance()->GetWindow()->GetWidth(), (float)Engine::GetInstance()->GetWindow()->GetHeight());

	glm::mat4 inverseViewProjection = glm::inverse(proj * view);

	glm::vec4 worldPosition = inverseViewProjection * glm::vec4(screenPosition, 0.0f, 1.0f);
	worldPosition /= worldPosition.w;
	
	return glm::vec2(worldPosition.x, worldPosition.y);
}

glm::vec2 Camera::WorldPosToScreenPos(glm::vec2 worldPosition)
{
	glm::mat4 view = GetViewMatrix();
	glm::mat4 proj = CalculateProjection((float)Engine::GetInstance()->GetWindow()->GetWidth(), (float)Engine::GetInstance()->GetWindow()->GetHeight());

	glm::vec4 screenPosition = proj * view * glm::vec4{worldPosition, 0.f, 1.f};
	screenPosition /= screenPosition.w;

	return glm::vec2(screenPosition.x, screenPosition.y);
}
