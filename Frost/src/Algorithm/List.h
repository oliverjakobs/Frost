#pragma once

#include <iostream>
#include <functional>

enum SORT_BEHAVIOUR
{
	SELECTION_SORT,
	INSERTION_SORT,
	BUBBLE_SORT,
	MERGE_SORT,
	QUICK_SORT,
	HEAP_SORT
};

template <typename Type>
struct ListElement
{
	Type data;
	ListElement<Type>* next;
	ListElement<Type>* prev;
};

template <typename Type>
class SimpleList
{
private:
	ListElement<Type>* first = nullptr;
	ListElement<Type>* last = nullptr;

	ListElement<Type>* get_element(int index);

	SORT_BEHAVIOUR sort_behaviour = HEAP_SORT;
	
	size_t element_count = 0;
public:
	SimpleList();
	SimpleList(Type* arr);
	SimpleList(SimpleList<Type> &copy);

	void clear();

	void insert(int index, Type data);
	void erase(int index);

	void push_back(Type data);
	void replace(int index, Type data);

	Type at(int index);

	Type front();
	Type back();

	void set_sort_behaviour(SORT_BEHAVIOUR behaviour);
	void sort();

	int size();
	bool is_empty();

	Type* to_array();
};

template<typename Type>
SimpleList<Type>::SimpleList() : element_count(0)
{

}

template<typename Type>
SimpleList<Type>::SimpleList(Type* arr)
{

}

template<typename Type>
SimpleList<Type>::SimpleList(SimpleList<Type> &copy)
{
	for (int i = 0; i < copy.size(); i++)
	{
		push_back(copy.at(i));
	}
}

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
void SimpleList<Type>::insert(int index, Type data)
{
	if (index < 0 || index > element_count)
	{
		std::string what_string = "Index " + std::to_string(index) + " out of range. Only indices from 0 to " + std::to_string(element_count - 1) + " allowed.";

		throw std::out_of_range(what_string);
	}
	else if (index == element_count)
	{
		push_back(data);
	}
	else
	{
		ListElement<Type>* before = get_element(index - 1);
		ListElement<Type>* after = get_element(index);
		ListElement<Type>* elem = new ListElement<Type>();
		elem->data = data;
		elem->next = after;
		elem->prev = before;

		before->next = elem;
		after->prev = elem;

		element_count++;
	}
}

template<typename Type>
void SimpleList<Type>::erase(int index)
{
	if (index < 0 || index >= element_count)
	{
		std::string what_string = "Index " + std::to_string(index) + " out of range. Only indices from 0 to " + std::to_string(element_count - 1) + " allowed.";

		throw std::out_of_range(what_string);
		return;
	}

	ListElement<Type>* before = get_element(index - 1);
	ListElement<Type>* elem = get_element(index);
	ListElement<Type>* after = get_element(index + 1);

	before->next = after;
	after->prev = before;

	element_count--;

	delete[] elem;
}

template<typename Type>
ListElement<Type>* SimpleList<Type>::get_element(int index)
{
	if (first == nullptr)
		return nullptr;

	if (index == 0)
		return first;

	if (index == element_count - 1)
		return last;

	if (index < 0 || index >= element_count)
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

	element_count++;
}

template<typename Type>
inline void SimpleList<Type>::replace(int index, Type data)
{
	if (index < 0 || index >= element_count)
	{
		std::string what_string = "Index " + std::to_string(index) + " out of range. Only indices from 0 to " + std::to_string(element_count - 1) + " allowed.";

		throw std::out_of_range(what_string);
		return;
	}

	get_element(index)->data = data;
}

template <typename Type>
Type SimpleList<Type>::at(int index)
{
	if (index < 0 || index >= element_count)
	{
		std::string what_string = "Index " + std::to_string(index) + " out of range. Only indices from 0 to " + std::to_string(element_count - 1) + " allowed.";
		throw std::out_of_range(what_string);
		return Type();
	}

	if (first == nullptr)
		return Type();

	if (index == 0)
		return front();

	if (index == element_count - 1)
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

template<typename Type>
inline void SimpleList<Type>::set_sort_behaviour(SORT_BEHAVIOUR behaviour)
{
	sort_behaviour = behaviour;
}

template<typename Type>
inline void SimpleList<Type>::sort()
{
	Type* arr = to_array();

	switch (sort_behaviour)
	{
	case SELECTION_SORT:
		selection_sort(arr, element_count);
		break;
	case INSERTION_SORT:
		insertion_sort(arr, element_count);
		break;
	case BUBBLE_SORT:
		bubble_sort(arr, element_count);
		break;
	case MERGE_SORT:
		merge_sort(arr, 0, element_count - 1);
		break;
	case QUICK_SORT:
		quick_sort(arr, 0, element_count - 1);
		break;
	case HEAP_SORT:
		heap_sort(arr, element_count);
		break;
	default:
		break;
	}

	for (int i = 0; i < element_count; i++)
	{
		cout << "arr[" << i << "]: " << arr[i] << endl;
		replace(i, arr[i]);
	}
}

template <typename Type>
int SimpleList<Type>::size()
{
	return element_count;
}

template <typename Type>
bool SimpleList<Type>::is_empty()
{
	return (first == nullptr);
}

template <typename Type>
Type* SimpleList<Type>::to_array()
{
	Type* arr = new Type[element_count];

	for (int i = 0; i < element_count; i++)
	{
		arr[i] = at(i);
	}

	return arr;
}