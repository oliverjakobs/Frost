#pragma once

#include "Stack.h"
#include "Queue.h"
#include "List.h"
#include "Sorting.h"
#include "BinaryTree.h"
#include "CustomSort.h"
#include <string>
#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

template<typename Type>
void print_array(char* msg, Type arr[], int size)
{
	cout << msg;

	for (int i = 0; i < size; i++)
	{
		cout << arr[i] << "  ";
	}

	cout << endl;
}

inline int algorithm_main()
{
	//list
	{
		SimpleList<int> list;

		cout << "list empty1 " << list.empty() << endl;

		list.push_back(1);
		list.push_back(2);
		list.push_back(3);
		list.push_back(4);

		cout << "list empty2 " << list.empty() << endl;

		cout << "list.at(0): " << list.at(0) << endl;
		cout << "list.at(1): " << list.at(1) << endl;
		cout << "list.at(2): " << list.at(2) << endl;
		cout << "list.at(3): " << list.at(3) << endl;
		try
		{
			cout << "list.at(4): " << list.at(4) << endl;
		}
		catch (out_of_range e)
		{
			cerr << "Out of range exception: " << e.what() << endl;
		}
		cout << endl;
	}

	//Custom List
	{
		
	}

	//Constructor
	{
		cout << "Constructor " << endl;

		SimpleList<int> list;

		list.push_back(1);
		list.push_back(45);
		list.push_back(24);
		list.push_back(37);
		list.push_back(6);

		cout << "list.at(0): " << list.at(0) << endl;
		cout << "list.at(1): " << list.at(1) << endl;
		cout << "list.at(2): " << list.at(2) << endl;
		cout << "list.at(3): " << list.at(3) << endl;
		cout << "list.at(4): " << list.at(4) << endl;

		SimpleList<int> list2 = SimpleList<int>(list);


		cout << "list2.at(0): " << list2.at(0) << endl;
		cout << "list2.at(1): " << list2.at(1) << endl;
		cout << "list2.at(2): " << list2.at(2) << endl;
		cout << "list2.at(3): " << list2.at(3) << endl;
		cout << "list2.at(4): " << list2.at(4) << endl;
	}

	//erase, insert
	{
		SimpleList<int> list;

		list.push_back(1);
		list.push_back(45);
		list.push_back(24);
		list.push_back(37);
		list.push_back(6);

		cout << "Erase " << endl;
		list.erase(2);

		cout << "list.at(0): " << list.at(0) << endl;
		cout << "list.at(1): " << list.at(1) << endl;
		cout << "list.at(2): " << list.at(2) << endl;
		cout << "list.at(3): " << list.at(3) << endl;
		try
		{
			cout << "list.at(4): " << list.at(4) << endl;
		}
		catch (out_of_range e)
		{
			cerr << "Out of range exception: " << e.what() << endl;
		}

		list.insert(3, 7);

		cout << "list.at(0): " << list.at(0) << endl;
		cout << "list.at(1): " << list.at(1) << endl;
		cout << "list.at(2): " << list.at(2) << endl;
		cout << "list.at(3): " << list.at(3) << endl;
		cout << "list.at(4): " << list.at(4) << endl;

	}

	//CustomSort
	{

		int list[] = { 96, 12, 3, 48, 8, 27, 2, 6, 27, 15, 34, 62, 19, 25 };
		int n = sizeof(list) / sizeof(list[0]);

		cout << "Custom sort: " << endl;

		print_array("unsorted: ", list, n);

		CustomSort<int> sort;

		sort.sort(list, n);

		print_array("sorted:   ", list, n);

		sort.sort_reverse(list, n);

		print_array("Reversed:   ", list, n);

		cout << endl;

	}

	int x;
	std::cin >> x;

	return 0;
}