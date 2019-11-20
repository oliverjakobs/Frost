#include "PlayerComponent.h"

#include "AnimationComponent.h"
#include "TextureComponent.h"
#include "PhysicsComponent.h"

#include "Obelisk/Obelisk.h"

#include "Input/Input.h"

PlayerComponent::PlayerComponent(Entity* entity, float ms, float jp)
	: Component(entity), m_movementSpeed(ms), m_jumpPower(jp)
{
}

void PlayerComponent::OnUpdate(float deltaTime)
{
	PhysicsComponent* phys = m_entity->GetComponent<PhysicsComponent>();
	OBELISK_ASSERT(phys, "[COMP] Entity has no PhysicsComponent");

	TextureComponent* tex = m_entity->GetComponent<TextureComponent>();
	OBELISK_ASSERT(tex, "[COMP] Entity has no TextureComponent");

	AnimationComponent* anim = m_entity->GetComponent<AnimationComponent>();
	OBELISK_ASSERT(anim, "[COMP] Entity has no AnimationComponent");


	glm::vec2 velocity = glm::vec2(0.0f, phys->GetBody()->GetVelocity().y);

	bool collidesBottom = phys->GetBody()->CollidesBottom();

	if (Input::KeyPressed(KEY_A))
		velocity.x -= m_movementSpeed;
	if (Input::KeyPressed(KEY_D))
		velocity.x += m_movementSpeed;

	if (Input::KeyPressed(KEY_SPACE) && collidesBottom)
		velocity.y = m_jumpPower;
	else if (Input::KeyReleased(KEY_SPACE) && (velocity.y > 0.0f))
		velocity.y *= 0.5f;

	if (Input::KeyPressed(KEY_S))
		phys->GetBody()->Drop();

	if (velocity.x > 0.0f)
	{
		m_entity->SetDirection(Direction::RIGHT);
		tex->SetRenderFlip(RenderFlip::NONE);
	}
	else if (velocity.x < 0.0f)
	{
		m_entity->SetDirection(Direction::LEFT);
		tex->SetRenderFlip(RenderFlip::HORIZONTAL);
	}

	// Animation
	if (velocity.y > 0.0f)
		anim->PlayAnimation("jump");
	else if (!collidesBottom)
		anim->PlayAnimation("fall");
	else if (velocity.x != 0.0f)
		anim->PlayAnimation("walk");
	else
		anim->PlayAnimation("idle");

	// apply velocity
	phys->GetBody()->SetVelocity(velocity);
}

void PlayerComponent::OnRender()
{
}

void PlayerComponent::OnRenderDebug()
{
}
