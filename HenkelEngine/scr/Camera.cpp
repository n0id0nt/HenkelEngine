#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up)
	: m_Postition(position), m_Up(up), m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_Fov(45.0f)
{
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ)
	: m_Postition(glm::vec3(posX, posY, posZ)), m_Up(glm::vec3(upX, upY, upZ)), m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_Fov(45.0f)
{
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::mat4(1.f);//glm::lookAt(m_Postition, m_Postition + m_Front, m_Up);
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

void Camera::SetPosition(glm::vec3 position)
{
	m_Postition = position;
}

glm::vec3 Camera::GetPosition()
{
	return m_Postition;
}

glm::mat4 Camera::CalculateProjection(float width, float height)
{
	if (m_Orthographic)
	{
		float left = m_Postition.x		- width		/ (2.f * m_zoom);
		float right = m_Postition.x		+ width		/ (2.f * m_zoom);
		float top = m_Postition.y		+ height	/ (2.f * m_zoom);
		float bottom = m_Postition.y	- height	/ (2.f * m_zoom);
		glm::mat4 orthoMatrix = glm::ortho(left, right, top, bottom);
		return orthoMatrix;
	}
	else
	{
		return glm::perspective(m_Fov, width / height, m_Near, m_Far);
	}
}
