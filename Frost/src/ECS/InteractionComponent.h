#pragma once

#include "Component.h"

#include <functional>

typedef std::function<void()> Interaction;

class InteractionComponent : public Component
{
private:
	float m_range;
	unsigned int m_interactionKey;

	bool m_interacting;
	Interaction m_interaction;
public:
	InteractionComponent(float range, unsigned int interactionKey, Interaction interaction);
	~InteractionComponent();

	void onInput() override;
	void onUpdate() override;
	void onRender() const override;
	void onRenderDebug() const override;
};