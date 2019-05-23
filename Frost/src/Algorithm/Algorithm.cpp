#include "Algorithm.h"

int BinarySearch(int field[], int object, int first, int last)
{
	if (last >= first)
	{
		int mid = (first + last) / 2;

		if (object == field[mid])
			return mid;

		if (object < field[mid])
			return BinarySearch(field, object, first, mid - 1);

		if (object > field[mid])
			return BinarySearch(field, object, mid + 1, last);
	}

	return -1;
}
