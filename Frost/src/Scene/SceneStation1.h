#pragma once

#include "SceneManager.h"

class SceneStation1 : public Scene
{
public:
	SceneStation1();

	virtual void onEntry() override;
	virtual void onExtit() override;
};