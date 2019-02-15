#pragma once

#include "Scrapbook/Utility.h"
#include "Scrapbook/Maths.h"

class Component;
class Scene;

enum EntityFlags
{
	FlagPlayer = 1 << 0,
	//Flag = 1 << 1,
	//C = 1 << 2,
	//D = 1 << 3
};

class Entity
{
protected:
	std::string m_name;

	glm::vec2 m_position;
	glm::vec2 m_dimension;

	sb::Flags m_flags;

	Scene* m_scene;

	std::vector<Component*> m_components;
public:
	Entity(const Entity& copy);
	Entity(const std::string& name, float x, float y, float w, float h);
	virtual ~Entity();

	virtual void onInput();
	virtual void onUpdate();
	virtual void onRender() const;
	virtual void onRenderDebug() const;

	void setScene(Scene* scene);
	Scene* getScene() const;

	void setFlag(unsigned int flag);
	bool hasFlag(unsigned int flag);

	void setPosition(glm::vec2 pos);
	glm::vec2 getPosition() const;
	glm::vec2 getCenter() const;

	float getWidth() const;
	float getHeight() const;
	glm::vec2 getSize() const;

	std::string getName() const;

	bool overlap(const Entity* entity) const;
	float getDistance(Entity* entity);
	Entity* Entity::getNearestEntity();

	Entity* addComponent(Component* c);
	std::vector<Component*> getComponents() const;

	template<class T> 
	T* getComponent() const
	{
		T* comp = nullptr;

		for (auto& c : m_components)
		{
			comp = dynamic_cast<T*>(c);
			if (comp != nullptr)
			{
				break;
			}
		}

		return comp;
	}
};