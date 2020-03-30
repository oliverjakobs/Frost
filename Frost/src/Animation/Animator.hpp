#pragma once

#include "Animation.hpp"

#include <unordered_map>
#include <functional>

struct Animator
{
	std::unordered_map<std::string, std::function<bool(Entity*, int)>> conditions;
	std::unordered_map<std::string, std::shared_ptr<Animation>> animations;
	std::string current;
};

void AnimatorInit(Animator* animator);
void AnimatorDestroy(Animator* animator);

void AnimatorRegisterConition(Animator* animator, const std::string& name, std::function<bool(Entity*, int)> condition);
void AnimatorCreateAnimation(Animator* animator, const std::string& name, int start, int length, float delay, std::vector<Transition> transitions);

void AnimatorTick(Animator* animator, Entity* entity, float deltatime);
void AnimatorPlay(Animator* animator, const std::string& name);

int AnimatorGetFrame(Animator* animator);