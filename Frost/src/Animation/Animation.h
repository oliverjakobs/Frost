#pragma once

#include <string>
#include <functional>
#include <map>

class Entity;

class Animation
{
private:
	int m_start;
	int m_length;

	float m_delay;
	float m_clock;

	int m_frame;

	std::map<std::string, std::string> m_transitions;

public:
	Animation() = default;
	Animation(int start, int length, float delay);

	void Start();
	void Tick(float deltaTime);

	void AddContition(const std::string& name, const std::string& next);

	int GetFrame() const { return m_frame; }
	const std::map<std::string, std::string> GetTransitions() const { return m_transitions; }

};