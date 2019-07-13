#pragma once

template<typename Type>
class Average
{
private:
	Type m_average;
	Type m_max;
	Type m_min;

	Type m_runningAverage;
	unsigned int m_count;

public:
	Average();
  
	void Set(Type value);

	Type GetAverage() const;
	Type GetMax() const;
	Type GetMin() const;
};
