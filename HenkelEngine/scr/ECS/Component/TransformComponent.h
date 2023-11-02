#pragma once

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include <sol\sol.hpp>

class TransformComponent
{
public:
	TransformComponent(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
	~TransformComponent();

	void SetPosition(glm::vec3 position);
	void SetPosition(glm::vec2 position);
	glm::vec3 GetPosition();	

	void SetWorldPosition(glm::vec3 position);
	void SetWorldPosition(glm::vec2 position);
	glm::vec3 GetWorldPosition();

	void SetRotation(glm::vec3 rotation);
	glm::vec3 GetRotation();
	
	void SetScale(glm::vec3 scale);
	glm::vec3 GetScale();

	void SetParent(TransformComponent* parent);
	TransformComponent* GetParent();

	glm::mat4 GetLocalMatrix();
	glm::mat4 GetWorldMatrix();

	static void LUABind(sol::state& lua);

private:

	void LUASetPosition(glm::vec2 position);

	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;

	TransformComponent* m_parent = nullptr;
};