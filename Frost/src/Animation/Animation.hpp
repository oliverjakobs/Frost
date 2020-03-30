#pragma once

#include <string>
#include <vector>

typedef std::pair<std::string, std::string> Transition;

struct Entity;

struct Animation
{
	int start;
	int length;

	float delay;
	float clock;

	int frame;

	std::vector<Transition> transitions;
};

void AnimationLoad(Animation* animation, int start, int length, float delay);

void AnimationStart(Animation* animation);
void AnimationTick(Animation* animation, float deltatime);

void AnimationAddTransition(Animation* animation, const std::string& name, const std::string& next);