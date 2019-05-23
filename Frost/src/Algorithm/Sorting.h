#pragma once

#include <algorithm>

template <typename Type>
void selection_sort(Type* list, int n)
{
	int min;

	for (int i = 0; i < n - 1; i++)
	{
		//set min to the current index of array
		min = i;

		for (int j = i + 1; j < n; j++)
		{
			if (list[j] < list[min])
				min = j;
		}

		//if min no longer equals i than a smaller value must have been found, so a swap must occur
		if (min != i)
		{
			std::swap(list[i], list[min]);
		}
	}
}

template <typename Type>
void insertion_sort(Type* list, int n)
{
	for (int i = 1; i < n; i++)
	{
		for (int j = i; j > 0 && list[j] < list[j - 1]; j--)
		{
			std::swap(list[j], list[j - 1]);
		}
	}
}

template <typename Type>
void bubble_sort(Type* list, int n)
{
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j < n - i - 1; j++)
		{
			if (list[j] > list[j + 1])
				std::swap(list[j], list[j + 1]);
		}
	}
}

template <typename Type>
void merge_sort(Type* list, int low, int high)
{
	int mid;

	if (low < high)
	{
		mid = (low + high) / 2;
		// Split the data into two half.
		merge_sort(list, low, mid);
		merge_sort(list, mid + 1, high);

		// Merge them to get sorted output.
		merge(list, low, mid, high);
	}
}

template <typename Type>
void merge(Type* list, int low, int mid, int high)
{
	// We have low to mid and mid+1 to high already sorted.
	Type* temp = new Type[high - low + 1];
	int i = low;
	int k = 0;
	int j = mid + 1;

	// Merge the two parts into temp[].
	while (i <= mid && j <= high)
	{
		if (list[i] < list[j])
		{
			temp[k++] = list[i++];
		}
		else
		{
			temp[k++] = list[j++];
		}
	}

	// Insert all the remaining values from i to mid into temp[].
	while (i <= mid)
	{
		temp[k] = list[i];
		k++;
		i++;
	}

	// Insert all the remaining values from j to high into temp[].
	while (j <= high)
	{
		temp[k] = list[j];
		k++;
		j++;
	}


	// Assign sorted data stored in temp[] to a[].
	for (i = low; i <= high; i++)
	{
		list[i] = temp[i - low];
	}
}

template <typename Type>
void quick_sort(Type* list, int low, int high)
{
	if (low < high)
	{
		Type pivot = list[high];
		int i = low;
		
		for (int j = low; j < high; j++)
		{
			if (list[j] <= pivot)
			{
				i++;
				std::swap(list[i - 1], list[j]);
			}
		}

		std::swap(list[i], list[high]);

		quick_sort(list, low, i - 1);
		quick_sort(list, i + 1, high);
	}
}

template <typename Type>
void heap_sort(Type* field, int n)
{
	//build heap (rearrange array)
	for (int i = n / 2 - 1; i >= 0; i--)
		sink(field, i, n);

	//move i-smallest element to field[n-1]
	//let new root sink in
	for (int i = n - 1; i >= 0; i--)
	{
		std::swap(field[0], field[i]);
		sink(field, 0, i);
	}
}

template <typename Type>
void sink(Type* field, int root, int n)
{
	int largest = root;  // Initialize largest as root
	int l = 2 * root + 1;  // left = 2*i + 1
	int r = 2 * root + 2;  // right = 2*i + 2

	// If left child is larger than root
	if (l < n && field[l] > field[largest])
		largest = l;

	// If right child is larger than largest so far
	if (r < n && field[r] > field[largest])
		largest = r;

	// If largest is not root
	if (largest != root)
	{
		std::swap(field[root], field[largest]);

		// Recursively heapify the affected sub-tree
		sink(field, largest, n);
	}
}