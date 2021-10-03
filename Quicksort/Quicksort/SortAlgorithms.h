#ifndef QUICKSORT_SORTALGORITHMS_H_
#define QUICKSORT_SORTALGORITHMS_H_

#include <functional>
#include <stack>
#include <tuple>

class SortAlgorithms {
public:
	// Get pointers to first element and last element (excluded), comparer function.
	// Implement insertion sort algorithm.
	template<typename T, typename Compare>
	static void InsertionSort(T first, T last, Compare comp) {
		if (first == last) return;

		for (T forward = first + 1; forward != last; ++forward) {
			T back = forward;

			for (; back > first && comp(*back, *(back - 1)); --back) {
				std::swap(*back, *(back - 1));
			}
		}
	}

	// Get pointers to first element and last element (excluded), comparer function.
	// Implement quick sort algorithm with some optimtzations.
	template<typename T, typename Compare>
	static void QuickSort(T first, T last, Compare comp) {
		int length = std::distance(first, last);

		if (length > 1) {
			if (length <= kShortIntervalLength) {
				InsertionSort(first, last, comp);
				return;
			}

			auto bounds = Partition(first, last, comp);		
			T left_bound = std::get<0>(bounds);
			T right_bound = std::get<1>(bounds);		

			if (std::distance(first, left_bound) > std::distance(right_bound, last)) {
				IterativeQuickSort(first, left_bound, comp);
				QuickSort(right_bound, last, comp);				
			}
			else {
				IterativeQuickSort(right_bound, last, comp);
				QuickSort(first, left_bound, comp);
			}
		}
	}

private:
	// Max interval length for insertion sort.
	static const int kShortIntervalLength = 10;

	// Get pointers to first element and last element (excluded), comparer function.
	// Implement iterative variation of quick sort algorithm.
	template<typename T, typename Compare>
	static void IterativeQuickSort(T first, T last, Compare comp) {
		std::stack<T> stack;
		stack.push(first);
		stack.push(last);

		while (!stack.empty()) {
			last = stack.top();
			stack.pop();
			first = stack.top();
			stack.pop();
			
			auto bounds = Partition(first, last, comp);
			int left_size = std::distance(first, std::get<0>(bounds));
			int right_size = std::distance(std::get<1>(bounds), last);

			if (left_size > 1) {
				if (left_size <= kShortIntervalLength) {
					InsertionSort(first, std::get<0>(bounds), comp);
				}
				else {
					stack.push(first);
					stack.push(std::get<0>(bounds));
				}
			}
			if (right_size > 1) {
				if (right_size <= kShortIntervalLength) {
					InsertionSort(std::get<1>(bounds), last, comp);
				}
				else {
					stack.push(std::get<1>(bounds));
					stack.push(last);
				}
			}
		}
	}

	// Get pointers to first element and last element (excluded), comparer function.
	// Return tuple which contain bounds for left and for right subsequences.
	// Method move elements, that greater than pivot, righter
	// and move elements, that less than pivot, lefter.
	template<typename T, typename Compare>
	static std::tuple<T, T> Partition(T first, T last, Compare comp) {
		auto pivot = GetPivot(first, last - 1, comp);
		int left = 0;
		int right = std::distance(first, last) - 1;

		for (; left <= right; ++left, --right) {
			for (; comp(first[left], pivot); ++left);
			for (; comp(pivot, first[right]); --right);
			if (left > right) break;
			std::swap(first[left], first[right]);
		}	

		return std::tuple<T, T>(std::next(first, right + 1), std::next(first, left));
	}

	// Get pointers to first, last elements and comparer function.
	// Return pivot for quick sort algorithm which calculated as median of
	// first, last and middle elements.
	template<typename T, typename Compare>
	static auto GetPivot(T first, T last, Compare comp) {
		T middle = first;
		std::advance(middle, std::distance(first, last) / 2);

		if (comp(*first, *middle)) {
			if (comp(*middle, *last)) return *middle;
			else if (comp(*first, *last)) return *last;
			else return *first;
		}
		else {
			if (comp(*first, *last)) return *first;
			else if (comp(*middle, *last)) return *last;
			else return *middle;
		}
	}
};

#endif // QUICKSORT_SORTALGORITHMS_H_