#pragma once

#include <iterator>

template <typename T>
struct range_t
{
	T b, e;
	range_t(T x, T y) : b(x), e(y) {}
	T begin()
	{
		return b;
	}
	T end()
	{
		return e;
	}
};

template <typename T>
range_t<T> range(T b, T e)
{
	return range_t<T>(b, e);
}

//Input: sorted field, object you're looking for, first index (mostly 0), last index (mostly the size of the field - 1) 
//Output: the index of the found object or -1 if the object wasn't found 
int BinarySearch(int field[], int object, int first, int last);