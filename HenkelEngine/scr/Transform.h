#pragma once

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

class Transform
{
public:
	Transform(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
	~Transform();

	void SetPosition(glm::vec3 position);
	glm::vec3 GetPosition();

	void SetRotation(glm::vec3 rotation);
	glm::vec3 GetRotation();
	
	void SetScale(glm::vec3 scale);
	glm::vec3 GetScale();

	void SetParent(Transform* parent);
	Transform* GetParent();

	glm::mat4 GetLocalMatrix();
	glm::mat4 GetWorldMatrix();
private:
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec3 m_Scale;

	Transform* m_Parent;
};