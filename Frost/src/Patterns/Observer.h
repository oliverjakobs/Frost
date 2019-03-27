#pragma once

#include <vector>
#include <memory>
#include <algorithm>

class Observer
{
public:
	virtual ~Observer() {}

	virtual void onNotify(unsigned int event) = 0;
};

class Subject
{
private:
	std::vector<std::shared_ptr<Observer>> m_observers;

protected:
	void notify(unsigned int event)
	{
		for (auto& obs : m_observers)
		{
			if (obs)
				obs->onNotify(event);
		}
	}

public:
	void addObserver(Observer* observer)
	{
		m_observers.push_back(std::make_shared<Observer>(observer));
	}

	void removeObserver(Observer* observer)
	{
		m_observers.erase(std::remove_if(m_observers.begin(), m_observers.end(), 
			[&](const auto& value) { return value == observer; }), m_observers.end());
	}
};