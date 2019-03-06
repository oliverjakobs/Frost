#pragma once

#include "TilePhysics/Tilemap.h"

class Scene
{
protected:
	unique_ptr<TileMap> m_map;
public:
	Scene(TileMap* map);
	~Scene();
	
	virtual void onEntry() {};
	virtual void onExtit() {};

	void onInput();
	void onUpdate();
	void onRender() const;
	void onRenderDebug() const;

	virtual void onUserInput() {};
	virtual void onUserUpdate() {};
	virtual void onUserRender() const {};
	virtual void onUserRenderDebug() const {};

	TileMap* getMap() const;
	Rect getConstraint() const;
};