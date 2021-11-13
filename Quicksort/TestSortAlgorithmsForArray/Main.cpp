#include "../../Array/Array/Array.h"
#include "../Quicksort/SortAlgorithms.h"

int main() {
	Array<int> arr;
	
	for (int i = 5; i >= 0; --i) {
		arr.Insert(i);
	}

	SortAlgorithms::OptimizedQuickSort(arr.begin(), arr.end(), std::less<int>());

	assert(std::is_sorted(arr.begin(), arr.end()));

	return 0;
}