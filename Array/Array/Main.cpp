#include <iostream>
#include "Array.h"

class Test {
public:
	int id_;

	Test(int id) 
		: id_(id)
	{}
};

int main() {
	Array<int> arr;
	arr.Insert(1);
	arr.Insert(2);
	arr.Insert(3);

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