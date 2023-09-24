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

private:
	glm::vec3 m_Postition;
	glm::vec3 m_Front;
	glm::vec3 m_Up;
	float m_Fov;
};

