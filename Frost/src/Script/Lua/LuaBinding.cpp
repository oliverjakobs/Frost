#include "LuaBinding.h"

#include "String/StringUtils.h"
#include "Input/Input.h"

#include "Debugger.h"
#include "Entity/Entity.h"
#include "Entity/Components.h"

unsigned int LuaInput::GetKeyCode(const std::string& key)
{
	if (stringCompare(key, "SPACE"))
		return KEY_SPACE;

	return key.front();
}

bool LuaInput::KeyPressed(const std::string& key)
{
	return Input::KeyPressed(GetKeyCode(key));
}

bool LuaInput::KeyReleased(const std::string & key)
{
	return Input::KeyReleased(GetKeyCode(key));
}

LuaEntity::LuaEntity(Entity* entity) : m_entity(entity)
{
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

void LuaEntity::Drop()
{
	PhysicsComponent* phys = m_entity->GetComponent<PhysicsComponent>();
	DEBUG_ASSERT(phys, "[LUA] Entity has no PhysicsComponent");

	phys->GetBody()->Drop();
}

void LuaEntity::SetVelocity(const glm::vec2& vel)
{
	PhysicsComponent* phys = m_entity->GetComponent<PhysicsComponent>();
	DEBUG_ASSERT(phys, "[LUA] Entity has no PhysicsComponent");

	phys->GetBody()->SetVelocity(vel);
}

void LuaEntity::PlayAnimation(const std::string& animation, int flip)
{
	AnimationComponent* anim = m_entity->GetComponent<AnimationComponent>();
	DEBUG_ASSERT(anim, "[LUA] Entity has no AnimationComponent");

	ImageComponent* img = m_entity->GetComponent<ImageComponent>();
	DEBUG_ASSERT(img, "[LUA] Entity has no ImageComponent");

	anim->PlayAnimation(animation);

	if (flip > 0)
		img->SetRenderFlip((RenderFlip)(flip - 1));
}

void LuaEntity::SetView()
{
	CameraComponent* cam = m_entity->GetComponent<CameraComponent>();
	DEBUG_ASSERT(cam, "[LUA] Entity has no CameraComponent");

	Renderer::SetViewCenter(m_entity->GetPosition() + cam->GetOffset(), cam->GetConstraint());
}

void LuaEntity::SetDirection(const std::string& direction)
{
	m_entity->SetDirection(Direction::FromString(direction));
}

void LuaBinding::LoadState()
{
	m_lua.open_libraries(sol::lib::base);

	m_lua.new_usertype<glm::vec2>("Vec2",
		sol::constructors<glm::vec2(), glm::vec2(float, float)>(),
		"x", &glm::vec2::x,
		"y", &glm::vec2::y
		);

	// Input
	m_lua.new_usertype<LuaInput>("Input",
		sol::constructors<LuaInput()>(),
		"KeyPressed", &LuaInput::KeyPressed,
		"KeyReleased", &LuaInput::KeyReleased
		);

	// Entity
	m_lua.new_usertype<LuaEntity>("Entity",
		sol::constructors<LuaEntity(Entity*)>(),
		"GetVelocity", &LuaEntity::GetVelocity,
		"CollidesBottom", &LuaEntity::CollidesBottom,
		"Drop", &LuaEntity::Drop,
		"SetVelocity", &LuaEntity::SetVelocity,
		"PlayAnimation", &LuaEntity::PlayAnimation,
		"SetView", &LuaEntity::SetView,
		"SetDirection", &LuaEntity::SetDirection
		);
}

sol::function LuaBinding::BindLuaFunction(const std::string& src, const std::string& func)
{
	m_lua.script_file(src);

	return m_lua[func];
}

sol::state& LuaBinding::GetState()
{
	return m_lua;
}