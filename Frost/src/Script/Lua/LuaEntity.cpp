#include "LuaEntity.h"

#include "Debugger.h"
#include "Scene/Scene.h"

LuaEntity::LuaEntity(Entity* entity) : m_entity(entity) { }

// base
void LuaEntity::SetPosition(const glm::vec2& pos)
{
	m_entity->SetPosition(pos);
}

void LuaEntity::SetDimension(const glm::vec2& dim)
{
	m_entity->SetDimension(dim);
}

void LuaEntity::SetDirection(const std::string& direction)
{
	m_entity->SetDirection(Direction::FromString(direction));
}

glm::vec2 LuaEntity::GetPosition()
{
	return m_entity->GetPosition();
}

glm::vec2 LuaEntity::GetDimension()
{
	return m_entity->GetDimension();
}

std::string LuaEntity::GetDirection()
{
	return Direction::ToString(m_entity->GetDirection());
}

// physics
void LuaEntity::SetVelocity(const glm::vec2& vel)
{
	PhysicsComponent* phys = m_entity->GetComponent<PhysicsComponent>();
	DEBUG_ASSERT(phys, "[LUA] Entity has no PhysicsComponent");

	phys->GetBody()->SetVelocity(vel);
}

glm::vec2 LuaEntity::GetVelocity()
{
	PhysicsComponent* phys = m_entity->GetComponent<PhysicsComponent>();
	DEBUG_ASSERT(phys, "[LUA] Entity has no PhysicsComponent");

	return phys->GetBody()->GetVelocity();
}

bool LuaEntity::CollidesBottom()
{
	PhysicsComponent* phys = m_entity->GetComponent<PhysicsComponent>();
	DEBUG_ASSERT(phys, "[LUA] Entity has no PhysicsComponent");

	return phys->GetBody()->CollidesBottom();
}

bool LuaEntity::CollidesTop()
{
	PhysicsComponent* phys = m_entity->GetComponent<PhysicsComponent>();
	DEBUG_ASSERT(phys, "[LUA] Entity has no PhysicsComponent");

	return phys->GetBody()->CollidesTop();
}

bool LuaEntity::CollidesLeft()
{
	PhysicsComponent* phys = m_entity->GetComponent<PhysicsComponent>();
	DEBUG_ASSERT(phys, "[LUA] Entity has no PhysicsComponent");

	return phys->GetBody()->CollidesLeft();
}

bool LuaEntity::CollidesRight()
{
	PhysicsComponent* phys = m_entity->GetComponent<PhysicsComponent>();
	DEBUG_ASSERT(phys, "[LUA] Entity has no PhysicsComponent");

	return phys->GetBody()->CollidesRight();
}

void LuaEntity::Drop()
{
	PhysicsComponent* phys = m_entity->GetComponent<PhysicsComponent>();
	DEBUG_ASSERT(phys, "[LUA] Entity has no PhysicsComponent");

	phys->GetBody()->Drop();
}

// graphics
void LuaEntity::PlayAnimation(const std::string& animation)
{
	AnimationComponent* anim = m_entity->GetComponent<AnimationComponent>();
	DEBUG_ASSERT(anim, "[LUA] Entity has no AnimationComponent");

	anim->PlayAnimation(animation);
}

void LuaEntity::SetRenderFlip(const std::string& flip)
{
	ImageComponent* img = m_entity->GetComponent<ImageComponent>();
	DEBUG_ASSERT(img, "[LUA] Entity has no ImageComponent");

	img->SetRenderFlip(RenderFlip::FromString(flip));
}

// view
void LuaEntity::CenterView(const glm::vec2& offset, bool lock)
{
	if (lock && m_entity->GetScene() != nullptr)
		View::SetCenter(m_entity->GetPosition() + offset, m_entity->GetScene()->GetConstraint());
	else
		View::SetCenter(m_entity->GetPosition() + offset);
}