#pragma once

template <class Type>
class SimpleStack
{
private:
	template <class Type>
	struct StackElement
	{
		Type data;
		StackElement* next;
	};

	StackElement<Type>* m_top;
public:
	SimpleStack();
	~SimpleStack();

	void clear();

	void push(Type t);
	Type pop();

	Type top() const;
	inline bool empty() const { return m_top == nullptr; }
};

template<class Type>
inline SimpleStack<Type>::SimpleStack() : m_top(nullptr) { }

template<class Type>
inline SimpleStack<Type>::~SimpleStack()
{
	clear();
}

template<class Type>
inline void SimpleStack<Type>::clear()
{
	// pop until empty
	while (!empty())
		pop();
}

template<class Type>
inline void SimpleStack<Type>::push(Type t)
{
	StackElement<Type>* temp = new StackElement<Type>();
	temp->data = t;
	temp->next = m_top;

	m_top = temp;
}

template<class Type>
inline Type SimpleStack<Type>::pop()
{
	if (m_top != nullptr)
	{
		Type data = m_top->data;
		auto next = m_top->next;
		delete m_top;
		m_top = next;

		return data;
	}

	return Type();
}

template<class Type>
inline Type SimpleStack<Type>::top() const
{
	if (empty())
		return Type();

	return m_top->data;
}
