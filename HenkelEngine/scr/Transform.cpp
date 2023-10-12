#include "Transform.h"

Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	: m_Position(position), m_Rotation(rotation), m_Scale(scale), m_Parent(nullptr)
{
}

Transform::~Transform()
{
}

void Transform::SetPosition(glm::vec3 position)
{
	m_Position = position;
}

glm::vec3 Transform::GetPosition()
{
	return m_Position;
}

void Transform::SetRotation(glm::vec3 rotation)
{
	m_Rotation = rotation;
}

glm::vec3 Transform::GetRotation()
{
	return m_Rotation;
}

void Transform::SetScale(glm::vec3 scale)
{
	m_Scale = scale;
}

glm::vec3 Transform::GetScale()
{
	return m_Scale;
}

void Transform::SetParent(Transform* parent)
{
	m_Parent = parent;
}

Transform* Transform::GetParent()
{
	return m_Parent;
}

glm::mat4 Transform::GetLocalMatrix()
{
	glm::mat4 matrix = glm::translate(glm::mat4(1.f), m_Position);

	matrix = glm::rotate(matrix, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	matrix = glm::rotate(matrix, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	matrix = glm::rotate(matrix, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	matrix = glm::scale(matrix, m_Scale);

	return matrix;
}

glm::mat4 Transform::GetWorldMatrix()
{
	if (m_Parent)
		return GetLocalMatrix() * m_Parent->GetWorldMatrix();
	else
		return GetLocalMatrix();
}
