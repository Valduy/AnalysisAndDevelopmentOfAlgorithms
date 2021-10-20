#include <iostream>
#include "Array.h"

int main() {
	Array<int> arr;
	arr.Insert(0);
	arr.Insert(0);
	arr.Insert(0);

	for (auto it : arr) {
		std::cout << it << " ";
	}
	std::cout << "\n";

	for (auto it = arr.begin(); it < arr.end(); ++it) {
		*it += 1;
	}

	for (auto it : arr) {
		std::cout << it << " ";
	}
	std::cout << "\n";

	return 0;
}