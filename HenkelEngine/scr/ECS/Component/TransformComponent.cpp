#include "TransformComponent.h"
#include "glm/gtx/matrix_decompose.hpp"
#include "imgui.h"
#include "PhysicsBodyComponents/PhysicsBodyComponent.h"
#include "PhysicsBodyComponents/StaticBodyComponent.h"
#include "PhysicsBodyComponents/TileMapCollisionBodyComponent.h"
#include <DebugGUIPanels/ImGuiHelper.h>

TransformComponent::TransformComponent(Entity* entity, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	: m_position(position), m_rotation(rotation), m_scale(scale), m_entity(entity)
{
	UpdateComponentsTransforms();
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::SetPosition(glm::vec3 position)
{
	m_position = position;
	UpdateComponentsTransforms();
}

glm::vec3 TransformComponent::GetPosition()
{
	return m_position;
}

glm::vec2 TransformComponent::GetPosition2()
{
	return GetPosition();
}

void TransformComponent::SetPosition(glm::vec2 position)
{
	m_position = { position, 0.f };
	UpdateComponentsTransforms();
}

void TransformComponent::SetWorldPosition(glm::vec3 position)
{
	m_position = position;
	if (GetParent())
		m_position -= GetParent()->GetWorldPosition();
	UpdateComponentsTransforms();
}

void TransformComponent::SetWorldPosition(glm::vec2 position)
{
	m_position = glm::vec3{ position, 0.f };
	if (GetParent())
		m_position -= GetParent()->GetWorldPosition();
	UpdateComponentsTransforms();
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

glm::vec2 TransformComponent::GetWorldPosition2()
{
	return GetWorldPosition();
}

void TransformComponent::SetRotation(glm::vec3 rotation)
{
	m_rotation = rotation;
	UpdateComponentsTransforms();
}

glm::vec3 TransformComponent::GetRotation()
{
	return m_rotation;
}

void TransformComponent::SetScale(glm::vec3 scale)
{
	m_scale = scale;
	UpdateComponentsTransforms();
}

glm::vec3 TransformComponent::GetScale()
{
	return m_scale;
}

TransformComponent* TransformComponent::GetParent()
{
	Entity* parent = m_entity->GetParent();
	if (parent)
		return parent->GetComponent<TransformComponent>();

	return nullptr;
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
	if (GetParent())
		return GetParent()->GetWorldMatrix() * GetLocalMatrix();
	else
		return GetLocalMatrix();
}

void TransformComponent::DrawDebugPanel()
{
	const float inputFieldWidth = 80.f;
	glm::vec2 pos = m_position;

	ImGui::DrawVec2Control("Position", pos);

	SetPosition(pos);
}

void TransformComponent::UpdateComponentsTransforms()
{
	glm::vec3 worldPosition = GetWorldPosition();
	auto* physicsBody = m_entity->GetComponent<PhysicsBodyComponent>();
	auto* staticBody = m_entity->GetComponent<StaticBodyComponent>();
	auto* tileMap = m_entity->GetComponent<TileMapCollisionBodyComponent>();
	if (physicsBody)
	{
		physicsBody->SetPosition(worldPosition);
	}
	if (staticBody)
	{
		staticBody->SetPosition(worldPosition);
	}
	if (tileMap)
	{
		tileMap->SetPosition(worldPosition);
	}

	for (auto& child : m_entity->GetChildren())
	{
		auto* transform = child->GetComponent<TransformComponent>();
		transform->UpdateComponentsTransforms();
	}
}

void TransformComponent::LUABind(sol::state& lua)
{
	lua.new_usertype<TransformComponent>("transform",
		"setPosition", sol::resolve<void(glm::vec2)>(&TransformComponent::SetPosition),
		"getPosition", &TransformComponent::GetPosition2,
		"setRotation", &TransformComponent::SetRotation,
		"getRotation", &TransformComponent::GetRotation,
		"setScale", &TransformComponent::SetScale,
		"getScale", &TransformComponent::GetScale,
		"setWorldPosition", sol::resolve<void(glm::vec2)>(&TransformComponent::SetWorldPosition),
		"getWorldPosition", &TransformComponent::GetWorldPosition2
	);
}