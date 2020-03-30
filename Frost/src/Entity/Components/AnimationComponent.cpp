#include "AnimationComponent.hpp"

AnimationComponent::AnimationComponent(Entity* entity, std::shared_ptr<Animator> animator)
	: Component(entity), m_animator(animator)
{
}


void AnimationComponent::OnUpdate(Scene* scene, float deltaTime)
{
	AnimatorTick(m_animator.get(), m_entity, deltaTime);

	TextureComponent* tex = EntityGetComponent<TextureComponent>(m_entity);

	if (tex != nullptr)
		tex->SetFrame(AnimatorGetFrame(m_animator.get()));
}
