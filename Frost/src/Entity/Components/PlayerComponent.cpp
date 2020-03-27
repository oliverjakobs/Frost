#include "PlayerComponent.hpp"

#include "AnimationComponent.hpp"
#include "TextureComponent.hpp"
#include "PhysicsComponent.hpp"

#include "Obelisk/Obelisk.hpp"

#include "Input/Input.hpp"

#include "../Scene.hpp"

PlayerComponent::PlayerComponent(Entity* entity, float ms, float jp)
	: Component(entity), m_movementSpeed(ms), m_jumpPower(jp)
{
}

void PlayerComponent::OnUpdate(Scene* scene, float deltaTime)
{
	PhysicsComponent* phys = EntityGetComponent<PhysicsComponent>(m_entity);
	OBELISK_ASSERT(phys, "[COMP] Entity has no PhysicsComponent");
	
	TextureComponent* tex = EntityGetComponent<TextureComponent>(m_entity);
	OBELISK_ASSERT(tex, "[COMP] Entity has no TextureComponent");
	
	glm::vec2 velocity = glm::vec2(0.0f, phys->GetBody()->GetVelocity().y);
	
	bool collidesBottom = phys->GetBody()->CollidesBottom();
	
	if (InputKeyPressed(KEY_A))
		velocity.x -= m_movementSpeed;
	if (InputKeyPressed(KEY_D))
		velocity.x += m_movementSpeed;
	
	if (InputKeyPressed(KEY_SPACE) && collidesBottom)
		velocity.y = m_jumpPower;
	else if (InputKeyReleased(KEY_SPACE) && (velocity.y > 0.0f))
		velocity.y *= 0.5f;
	
	if (velocity.x > 0.0f)
	{
		m_entity->direction = Direction::RIGHT;
		tex->SetRenderFlip(RenderFlip::NONE);
	}
	else if (velocity.x < 0.0f)
	{
		m_entity->direction = Direction::LEFT;
		tex->SetRenderFlip(RenderFlip::HORIZONTAL);
	}
	
	// Animation
	// if (velocity.y > 0.0f)
	// 	anim->PlayAnimation("jump");
	// else if (!collidesBottom)
	// 	anim->PlayAnimation("fall");
	// else if (velocity.x != 0.0f)
	// 	anim->PlayAnimation("walk");
	// else
	// 	anim->PlayAnimation("idle");
	
	// apply velocity
	phys->GetBody()->SetVelocity(velocity);
	
	// set view
	SceneSetCameraPosition(scene, glm::vec3(EntityGetPosition(m_entity), 0.0f));
}
