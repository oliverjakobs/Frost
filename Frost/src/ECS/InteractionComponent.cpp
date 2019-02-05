#include "InteractionComponent.h"
#include "Scene/Scene.h"

InteractionComponent::InteractionComponent(float range, unsigned int interactionKey, Interaction interaction)
	: m_range(range), m_interactionKey(interactionKey), m_interaction(interaction), m_interacting(false)
{
}

InteractionComponent::~InteractionComponent()
{
}

void InteractionComponent::onInput()
{
	if (Input::KeyPressed(m_interactionKey))
	{
		Entity* entity = m_entity->getNearestEntity();

		if (m_entity->getDistance(entity) < m_range)
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
	Renderer::DrawCircle(m_entity->getPosition(), m_range, GREEN);
}
