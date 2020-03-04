#include "AnimationComponent.h"

AnimationComponent::AnimationComponent(Entity* entity, std::shared_ptr<Animator> animator)
	: Component(entity), m_animator(animator)
{
}


void AnimationComponent::OnUpdate(Scene* scene, float deltaTime)
{
	m_animator->Tick(m_entity, deltaTime);

	TextureComponent* tex = m_entity->GetComponent<TextureComponent>();

	if (tex != nullptr)
		tex->SetFrame(m_animator->GetFrame());
}
