#ifndef QUICKSORT_SORTALGORITHMS_H_
#define QUICKSORT_SORTALGORITHMS_H_

class SortAlgorithms {
public:
	// Get pointers to first element and last element (excluded), comparer function.
	// Implement insertion sort algorithm.
	template<typename T, typename Compare>
	static void InsertionSort(T first, T last, Compare comp) {
		if (std::distance(first, last) <= 1) return;

		for (T forward = std::next(first, 1); forward != last; std::advance(forward, 1)) {	
			auto temp = std::move(*forward);
			T back = forward;

			while (back != first && comp(temp, *std::prev(back))) {
				*back = std::move(*std::prev(back));
				std::advance(back, -1);
			}

			*back = std::move(temp);
		}
	}

	// Get pointers to first element and last element (excluded), comparer function.
	// Implement quick sort algorithm.
	template<typename T, typename Compare>
	static void QuickSort(T first, T last, Compare comp) {
		if (std::distance(first, last) > 1) {
			auto bound = Partition(first, last - 1, comp) + 1;
			QuickSort(first, bound, comp);
			QuickSort(bound, last, comp);
		}
	}

	// Get pointers to first element and last element (excluded), comparer function.
	// Implement quick sort with tail recursion elimination.
	template<typename T, typename Compare>
	static void OptimizedQuickSort(T first, T last, Compare comp) {
		while (true) {
			if (std::distance(first, last) <= kShortIntervalLength) {
				InsertionSort(first, last, comp);
				break;
			}

			auto bound = std::next(Partition(first, std::next(last, -1), comp), 1);

			if (std::distance(first, bound) > std::distance(bound, last)) {
				OptimizedQuickSort(bound, last, comp);
				last = bound;
			}
			else {
				OptimizedQuickSort(first, bound, comp);
				first = bound;
			}
		}
	}

private:
	// Max interval length for insertion sort.
	static const int kShortIntervalLength = 16;

	// Get pointers to first element, last element and comparer function.
	// Return pointer to bound of partition.
	// Method move elements, that greater than pivot, righter
	// and move elements, that less than pivot, lefter.
	template<typename T, typename Compare>
	static T Partition(T first, T last, Compare comp) {
		auto pivot = GetPivot(first, last, comp);

		for (; first != std::next(last); std::advance(first, 1), std::advance(last, -1)) {
			for (; comp(*first, pivot); std::advance(first, 1));
			for (; comp(pivot, *last); std::advance(last, -1));
			if (first == last || first == std::next(last)) break;
			Swap(*first, *last);
		}

		return last;
	}

	// Get pointers to first element, last element and comparer function.
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

	// Get references to elements, which should be swapped.
	// Method implement swap algorithm.
	template<typename T>
	static void Swap(T& t1, T& t2) {
		T temp = std::move(t1);
		t1 = std::move(t2);
		t2 = std::move(temp);
	}
};

#endif // QUICKSORT_SORTALGORITHMS_H_