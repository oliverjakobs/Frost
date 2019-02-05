#pragma once

#include "Scrapbook/Graphics.h"
#include "PhysicsComponent.h"

class Animation
{
private:
	int m_start;
	int m_length;

	float m_delay;
	float m_frameCounter;

	int m_frame;
public:
	Animation(int start, int length, float delay);
	virtual ~Animation();

	void setStart(int s);

	void start();
	void step(float deltaTime);
	int getFrame();
};

typedef std::pair<std::string, Animation*> AnimationDef;

class AnimationComponent : public Component
{
private:
	std::map<std::string, Animation*> m_animations;
	Animation* m_currentAnimation;
	std::string m_currentName;

	Image* m_sprite;
	PhysicsComponent* m_physComp;
public:
	AnimationComponent(Image* sprite, std::map<std::string, Animation*> animations);
	~AnimationComponent();

	void flip(RenderFlip flip);
	void play(const std::string anim);

	bool setEntity(Entity* e) override;

	void onInput() override;
	void onUpdate() override;
	void onRender() const override;
};