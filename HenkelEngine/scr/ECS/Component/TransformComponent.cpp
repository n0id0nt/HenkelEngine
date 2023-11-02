#include "TransformComponent.h"
#include "glm\gtx\matrix_decompose.hpp"

TransformComponent::TransformComponent(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	: m_position(position), m_rotation(rotation), m_scale(scale), m_parent(nullptr)
{
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::SetPosition(glm::vec3 position)
{
	m_position = position;
}

glm::vec3 TransformComponent::GetPosition()
{
	return m_position;
}

void TransformComponent::SetPosition(glm::vec2 position)
{
	m_position = { position, 0.f };
}

void TransformComponent::SetWorldPosition(glm::vec3 position)
{
	m_position = position - m_parent->GetWorldPosition();
}

void TransformComponent::SetWorldPosition(glm::vec2 position)
{
	m_position = glm::vec3{ position, 0.f };
	if (m_parent)
		m_position -= m_parent->GetWorldPosition();
}

glm::vec3 TransformComponent::GetWorldPosition()
{
	glm::vec3 scale;
	glm::quat rotation{};
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(GetWorldMatrix(), scale, rotation, translation, skew, perspective);
	return translation;
}

void TransformComponent::SetRotation(glm::vec3 rotation)
{
	m_rotation = rotation;
}

glm::vec3 TransformComponent::GetRotation()
{
	return m_rotation;
}

void TransformComponent::SetScale(glm::vec3 scale)
{
	m_scale = scale;
}

glm::vec3 TransformComponent::GetScale()
{
	return m_scale;
}

void TransformComponent::SetParent(TransformComponent* parent)
{
	m_parent = parent;
}

TransformComponent* TransformComponent::GetParent()
{
	return m_parent;
}

glm::mat4 TransformComponent::GetLocalMatrix()
{
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_scale);

	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	rotation = glm::rotate(rotation, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	rotation = glm::rotate(rotation, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	
	glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_position);

	glm::mat4 modelMatrix = translation * rotation * scale;

	return modelMatrix;
}

glm::mat4 TransformComponent::GetWorldMatrix()
{
	if (m_parent)
		return m_parent->GetWorldMatrix() * GetLocalMatrix();
	else
		return GetLocalMatrix();
}

void TransformComponent::LUABind(sol::state& lua)
{
	lua.new_usertype<TransformComponent>("transform",
		"setPosition", &TransformComponent::LUASetPosition,
		"getPosition", &TransformComponent::GetPosition,
		"setRotation", &TransformComponent::SetRotation,
		"getRotation", &TransformComponent::GetRotation,
		"setScale", &TransformComponent::SetScale,
		"getScale", &TransformComponent::GetScale
	);
}

void TransformComponent::LUASetPosition(glm::vec2 position)
{
	SetPosition(position);
}
