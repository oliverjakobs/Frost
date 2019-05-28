#pragma once

#include <functional>

template <typename Type>
class CustomSort
{
private:
	std::function<bool(Type, Type)> equal_func;
	std::function<bool(Type, Type)> less_func;
	std::function<bool(Type, Type)> greater_func;

	void sink(Type field[], int root, int n);
	void sink_reverse(Type field[], int root, int n);
public:
	CustomSort();

	void set_equal_function(std::function<bool(Type, Type)> equal)
	{
		equal_func = equal;
	}

	void set_less_function(std::function<bool(Type, Type)> less)
	{
		less_func = less;
	}

	void set_greater_function(std::function<bool(Type, Type)> greater)
	{
		greater_func = greater;
	}

	void sort(Type field[], int n);
	void sort_reverse(Type field[], int n);
};

template <typename Type>
CustomSort<Type>::CustomSort()
{
	equal_func = [](Type t1, Type  t2) { return t1 == t2; };
	less_func = [](Type t1, Type  t2) { return t1 < t2; };
	greater_func = [](Type t1, Type  t2) { return t1 > t2; };
}

template <typename Type>
void CustomSort<Type>::sink(Type field[], int root, int n)
{
	int largest = root;  // Initialize largest as root
	int l = 2 * root + 1;  // left = 2*i + 1
	int r = 2 * root + 2;  // right = 2*i + 2
						   
	// If left child is larger than root
	if (l < n && greater_func(field[l], field[largest]))
		largest = l;

	// If right child is larger than largest so far
	if (r < n && greater_func(field[r], field[largest]))
		largest = r;

	// If largest is not root
	if (largest != root)
	{
		swap(field[root], field[largest]);

		// Recursively heapify the affected sub-tree
		sink(field, largest, n);
	}
}

template <typename Type>
void CustomSort<Type>::sink_reverse(Type field[], int root, int n)
{
	int largest = root;  // Initialize largest as root
	int l = 2 * root + 1;  // left = 2*i + 1
	int r = 2 * root + 2;  // right = 2*i + 2

	// If left child is larger than root
	if (l < n && less_func(field[l], field[largest]))
		largest = l;

	// If right child is larger than largest so far
	if (r < n && less_func(field[r], field[largest]))
		largest = r;

	// If largest is not root
	if (largest != root)
	{
		swap(field[root], field[largest]);

		// Recursively heapify the affected sub-tree
		sink(field, largest, n);
	}
}

template <typename Type>
void CustomSort<Type>::sort(Type field[], int n)
{
	// build heap (rearrange array)
	for (int i = n / 2 - 1; i >= 0; i--)
		sink(field, i, n);

	// move i-smallest element to field[n-1] let new root sink in
	for (int i = n - 1; i >= 0; i--)
	{
		swap(field[0], field[i]);
		sink(field, 0, i);
	}
}

template <typename Type>
void CustomSort<Type>::sort_reverse(Type field[], int n)
{
	//build heap (rearrange array)
	for (int i = n / 2 - 1; i >= 0; i--)
		sink_reverse(field, i, n);

	//move i-smallest element to field[n-1]
	//let new root sink in
	for (int i = n - 1; i >= 0; i--)
	{
		swap(field[0], field[i]);

		sink_reverse(field, 0, i);
	}
}