#pragma once

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

class Camera
{
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ);

	glm::mat4 GetViewMatrix();
	float GetFov();

	void SetOrthographic(bool value);
	bool GetOrthographic();

	void SetZoom(float zoom);
	float GetZoom();

	void SetPosition(glm::vec3 position);
	glm::vec3 GetPosition();

	glm::mat4 CalculateProjection(float width, float height);

private:
	glm::vec3 m_Postition;
	glm::vec3 m_Front;
	glm::vec3 m_Up;
	float m_Fov;
	bool m_Orthographic;
	float m_Near = 0.1f;
	float m_Far = 1000.f;
	float m_zoom = 1.f;
};

