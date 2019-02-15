#include "InteractionComponent.h"
#include "Scene/Scene.h"

InteractionComponent::InteractionComponent(float range, unsigned int interactionKey, Interaction interaction)
	: m_range(range), m_interactionKey(interactionKey), m_interaction(interaction), m_interacting(false)
{
}

InteractionComponent::~InteractionComponent()
{
}

InteractionComponent* InteractionComponent::clone()
{
	return new InteractionComponent(*this);
}

void InteractionComponent::onInput()
{
	if (Input::KeyPressed(m_interactionKey))
	{
		Entity* entity = m_entity->getNearestEntity();

		if ((m_range <= 0.0f && m_entity->overlap(entity)) || (m_entity->getDistance(entity) < m_range))
		{
			m_interacting = true;
		}
	}
}

void InteractionComponent::onUpdate()
{
	if (m_interacting)
	{
		m_interaction();
		m_interacting = false;
	}
}

void InteractionComponent::onRender() const
{
}

void InteractionComponent::onRenderDebug() const
{
	if (m_range > 0.0f)
		Renderer::DrawCircle(m_entity->getPosition(), m_range, GREEN);
}
