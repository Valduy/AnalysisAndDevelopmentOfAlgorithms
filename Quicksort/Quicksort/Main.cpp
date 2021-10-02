#include <iostream>
#include <vector>
#include <algorithm>
#include "SortAlgorithms.h"
#include "Timer.h"
#include "Random.h"

template<typename T>
std::vector<T> CreateVector(std::function<T()> gen, int size) {
	std::vector<T> result(size);
	std::generate(result.begin(), result.end(), gen);
	return result;
}

int main() {
	Timer timer;
	Random random(0, std::numeric_limits<int>::max());
	int vector_size = 1000;
	int vectors_count = 1000;

	std::cout << "Let's start...\n";	

	auto random_vectors = CreateVector<std::vector<int>>(
		[&]() {
			return CreateVector<int>([&]() { 
				return random.Next(); 
			}, vector_size);
		}, vectors_count);

	std::cout << "Sequences initialized.\n";
	std::cout << "Let's measure time...\n";

	timer.Start();

	for (auto sequence : random_vectors) {
		SortAlgorithms::QuickSort(sequence.begin(), sequence.end(), std::less<int>());
	}

	timer.Stop();

	std::cout << "Stop measuring.\n";
	std::cout 
		<< "For " << vectors_count 
		<< " sequences of " << vector_size << " elements " 
		<< timer.EllapsedMilliseconds() << " ms have elapsed\n";

	return 0;
}