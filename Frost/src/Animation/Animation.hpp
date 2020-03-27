#pragma once

#include <string>
#include <vector>

typedef std::pair<std::string, std::string> Transition;

struct Entity;

class Animation
{
private:
	int m_start;
	int m_length;

	float m_delay;
	float m_clock;

	int m_frame;

	std::vector<Transition> m_transitions;

public:
	Animation() = default;
	Animation(int start, int length, float delay);

	void Start();
	void Tick(float deltaTime);

	void AddTransition(const std::string& name, const std::string& next);
	const std::vector<Transition> GetTransitions() const { return m_transitions; }

	int GetFrame() const { return m_frame; }
};