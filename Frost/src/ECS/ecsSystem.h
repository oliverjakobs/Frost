#pragma once

class ecsSystem
{
public:
	virtual ~ecsSystem() = default;

	virtual bool init() = 0;
	virtual void update() = 0;
};
