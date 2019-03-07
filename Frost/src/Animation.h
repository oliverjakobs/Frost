#pragma once

#include <map>
#include <string>

class Animation
{
private:
	int m_start;
	int m_length;

	float m_delay;
	float m_frameCounter;

	int m_frame;
public:
	Animation();
	Animation(int start, int length, float delay);
	~Animation();

	void setStart(int s);

	void start();
	void step(float deltaTime);
	int getFrame();
};

typedef std::pair<std::string, Animation> AnimationDef;