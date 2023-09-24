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
	return glm::lookAt(m_Postition, m_Postition + m_Front, m_Up);
}

float Camera::GetFov()
{
	return m_Fov;
}
