#pragma once

#include <string>
#include <vector>
#include <glm\glm.hpp>

class Component;

class Entity
{
protected:
	std::string m_name;

	glm::vec2 m_position;
	glm::vec2 m_dimension;

	std::vector<Component*> m_components;
public:
	Entity(const std::string& name, float x, float y, float w, float h);
	virtual ~Entity();

	virtual void onInput();
	virtual void onUpdate();
	virtual void onRender() const;
	virtual void onRenderDebug() const;

	void setPosition(glm::vec2 pos);
	glm::vec2 getPosition() const;
	glm::vec2 getCenter() const;

	float getWidth() const;
	float getHeight() const;
	glm::vec2 getSize() const;

	std::string getName() const;

	void addComponent(Component* c);
	std::vector<Component*> getComponents() const;
};