#include "../../Array/Array/Array.h"
#include "../Quicksort/SortAlgorithms.h"
#include <list>
#include <iostream>

int main() {
	Array<int> arr;
	//std::list<int> arr;

	for (int i = 5; i >= 0; --i) {
		arr.Insert(i);
		//arr.push_back(i);
	}

	SortAlgorithms::OptimizedQuickSort(arr.begin(), arr.end(), std::less<int>());

	assert(std::is_sorted(arr.begin(), arr.end()));

	return 0;
}