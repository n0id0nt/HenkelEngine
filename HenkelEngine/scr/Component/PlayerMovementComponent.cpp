#include "PlayerMovementComponent.h"
#include "../Input.h"
#include "../Entity/Entity.h"

const float speed = 80.f;

PlayerMovementComponent::PlayerMovementComponent(Entity* entity) : Component(entity)
{
}

void PlayerMovementComponent::Update(float deltaTime)
{
	float horrizontalDir = Input::GetArrowDir().x;

	glm::vec2 velocity = glm::vec2{ horrizontalDir, 0.f } * speed * deltaTime;
	glm::vec3 newPos = GetEntity()->GetTransform()->GetPosition() + glm::vec3{ velocity, 0.f };
	GetEntity()->GetTransform()->SetPosition(newPos);

}
