#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>
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
	std::ofstream fout("Results.csv");
	Timer timer;
	Random random(0, std::numeric_limits<int>::max());
	int count_of_runs = 1000;

	std::cout << "Let's start.\n";

	for (int size = 0; size <= 100; size++) {
		std::cout << "Size: " << size << "\n";

		std::vector<int> insertion_sort_times;
		std::vector<int> quick_sort_times;

		auto random_vectors = CreateVector<std::vector<int>>(
			[&]() {
				return CreateVector<int>([&]() {
					return random.Next();
					}, size);
			}, count_of_runs);

		for (auto sequence : random_vectors) {
			auto copy_4_insertions_sort = std::vector<int>(sequence);
			
			timer.Start();
			SortAlgorithms::InsertionSort(copy_4_insertions_sort.begin(), copy_4_insertions_sort.end(), std::less<int>());
			timer.Stop();

			insertion_sort_times.push_back(timer.EllapsedMilliseconds());

			auto copy_4_quick_sort = std::vector<int>(sequence);

			timer.Start();
			SortAlgorithms::QuickSort(copy_4_insertions_sort.begin(), copy_4_insertions_sort.end(), std::less<int>());
			timer.Stop();

			quick_sort_times.push_back(timer.EllapsedMilliseconds());
		}
		
		fout 
			<< std::accumulate(insertion_sort_times.begin(), insertion_sort_times.end(), 0.0) / insertion_sort_times.size()
			<< ";"
			<< std::accumulate(quick_sort_times.begin(), quick_sort_times.end(), 0.0) / quick_sort_times.size()
			<< ";\n";
	}

	std::cout << "Finished\n.";

	return 0;
}