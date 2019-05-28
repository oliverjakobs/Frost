#pragma once

#include <iostream>
#include <functional>

template <typename Type>
class SimpleList
{
private:
	template <typename Type>
	struct ListElement
	{
		Type data;
		ListElement<Type>* next;
		ListElement<Type>* prev;
	};

	ListElement<Type>* first = nullptr;
	ListElement<Type>* last = nullptr;

	size_t m_size = 0;
	
	inline ListElement<Type>* get(size_t index)
	{
		if (first == nullptr)
			return nullptr;

		if (index == 0)
			return first;

		if (index == m_size - 1)
			return last;

		if (index >= m_size)
			return nullptr;

		ListElement<Type>* temp = first;

		int i = 0;

		while (temp != nullptr)
		{
			if (i == index)
			{
				return temp;
			}

			temp = temp->next;
			i++;
		}

		return nullptr;
	}
public:
	SimpleList() = default;
	//SimpleList(Type* arr);
	//SimpleList(SimpleList<Type> &copy);

	void clear();

	void push_back(Type data);

	void insert(size_t index, Type data);
	void replace(size_t index, Type data);
	void erase(size_t index);

	Type at(size_t index);

	Type front();
	Type back();

	size_t size();
	bool empty();

	Type* data();
};

template<typename Type>
void SimpleList<Type>::clear()
{
	ListElement<Type>* pointer = first;
	ListElement<Type>* temp;

	while(pointer != nullptr)
	{
		temp = pointer->next;
		delete[] pointer;
		pointer = temp;
	}

	first = nullptr;
	element_count = 0;
}

template<typename Type>
inline void SimpleList<Type>::insert(size_t index, Type data)
{
	if (index > m_size)
	{
		std::string what_string = "Index " + std::to_string(index) + " out of range. Only indices from 0 to " + std::to_string(m_size - 1) + " allowed.";

		throw std::out_of_range(what_string);
	}
	else if (index == m_size)
	{
		push_back(data);
	}
	else
	{
		ListElement<Type>* before = get(index - 1);
		ListElement<Type>* after = get(index);
		ListElement<Type>* elem = new ListElement<Type>();
		elem->data = data;
		elem->next = after;
		elem->prev = before;

		before->next = elem;
		after->prev = elem;

		m_size++;
	}
}

template<typename Type>
void SimpleList<Type>::erase(size_t index)
{
	if (index >= m_size)
	{
		std::string what_string = "Index " + std::to_string(index) + " out of range. Only indices from 0 to " + std::to_string(m_size - 1) + " allowed.";

		throw std::out_of_range(what_string);
		return;
	}

	ListElement<Type>* before = get(index - 1);
	ListElement<Type>* elem = get(index);
	ListElement<Type>* after = get(index + 1);

	before->next = after;
	after->prev = before;

	m_size--;

	delete elem;
}

template <typename Type>
void SimpleList<Type>::push_back(Type data)
{
	ListElement<Type>* temp = new ListElement<Type>();

	temp->data = data;
	temp->prev = last;
	temp->next = nullptr;

	if (first == nullptr)
	{
		first = temp;
	}
	else if (last == nullptr)
	{
		temp->prev = first;
		last = temp;

		first->next = last;
	}
	else
	{
		last->next = temp;
		last = temp;
	}

	m_size++;
}

template<typename Type>
inline void SimpleList<Type>::replace(size_t index, Type data)
{
	if (index >= element_count)
	{
		std::string what_string = "Index " + std::to_string(index) + " out of range. Only indices from 0 to " + std::to_string(element_count - 1) + " allowed.";

		throw std::out_of_range(what_string);
		return;
	}

	get(index)->data = data;
}

template <typename Type>
Type SimpleList<Type>::at(size_t index)
{
	if (index >= m_size)
	{
		std::string what_string = "Index " + std::to_string(index) + " out of range. Only indices from 0 to " + std::to_string(m_size - 1) + " allowed.";
		throw std::out_of_range(what_string);
		return Type();
	}

	if (first == nullptr)
		return Type();

	if (index == 0)
		return front();

	if (index == m_size - 1)
		return back();

	ListElement<Type>* temp = first;

	int i = 0;

	while (temp != nullptr)
	{
		if (i == index)
		{
			return temp->data;
		}

		temp = temp->next;
		i++;
	}

	return Type();
}

template <typename Type>
Type SimpleList<Type>::front()
{
	if (first != nullptr)
		return first->data;

	return Type();
}

template <typename Type>
Type SimpleList<Type>::back()
{
	if (last != nullptr)
		return last->data;

	return Type();
}


template <typename Type>
size_t SimpleList<Type>::size()
{
	return element_count;
}

template <typename Type>
bool SimpleList<Type>::empty()
{
	return (first == nullptr);
}

template <typename Type>
Type* SimpleList<Type>::data()
{
	Type* arr = new Type[m_size];

	for (int i = 0; i < m_size; i++)
	{
		arr[i] = at(i);
	}

	return arr;
}