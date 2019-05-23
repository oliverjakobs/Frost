#pragma once

template <class Type>
class SimpleQueue
{
private:
	template <class Type>
	struct QueueElement
	{
		Type data;
		QueueElement* next;
	};

	QueueElement<Type>* m_first;
	QueueElement<Type>* m_last;
public:
	SimpleQueue();
	~SimpleQueue();

	void clear();

	void push(Type t);
	Type pop();

	inline bool empty() const { return m_first == nullptr && m_last == nullptr; }
};

template<class Type>
inline SimpleQueue<Type>::SimpleQueue() : m_first(nullptr), m_last(nullptr) { }

template<class Type>
inline SimpleQueue<Type>::~SimpleQueue()
{
	clear();
}

template<class Type>
inline void SimpleQueue<Type>::clear()
{
	// pop until empty
	while (!empty())
		pop();
}

template<class Type>
inline void SimpleQueue<Type>::push(Type t)
{
	QueueElement<Type>* element = new QueueElement<Type>();
	element->data = t;
	element->next = nullptr;

	if (empty())
	{
		m_first = element;
		m_last = element;
	}
	else
	{
		m_last->next = element;
		m_last = element;
	}
}

template<class Type>
inline Type SimpleQueue<Type>::pop()
{
	if (m_first != nullptr)
	{
		Type data = m_first->data;
		auto next = m_first->next;
		delete m_first;
		m_first = next;

		if (m_first == nullptr)
			m_last = nullptr;

		return data;
	}

	return Type();
}
