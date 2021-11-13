#include "pch.h"
#include "../Quicksort/SortAlgorithms.h"
#include "gtest/gtest.h"

namespace {
using testing::Types;

template<typename T>
using Items = std::vector<T>;

template<typename T>
using Params = std::vector<Items<T>>;

// Declare factory for fixture parameters.
template<typename T>
Params<T> CreateParams();

// Let's specify concrete parameters for different types.
template<>
Params<int> CreateParams() {
	return {
		Items<int>(), // Empty.
		Items<int> { 4 }, // Single element.
		Items<int> { 1, 2, 3, 4, 5 }, // Sorted (odd count).
		Items<int> { 1, 2, 3, 4, 5, 6 }, // Sorted (even count).
		Items<int> { 7, 6, 5, 4, 3 }, // Sorted in reverce order (odd count).
		Items<int> { 8, 7, 6, 5, 4, 3 }, // Sorted in reverce order (odd count).
		Items<int> { 8, 4, 3, 6, 2, 15, 10, 0, -5, 18, 101, 19, 29, 31, 31, -15, 75 }, // Arbitrary sequence (odd count).
		Items<int> { 3, 2, 7, 15, -8, 25, 1, 1, 6, 9, -3, 28, 73, -31, -8, 41 }, // Arbitrary sequence (even count).
	};
}

template<>
Params<std::string> CreateParams() {
	return {
		Items<std::string>(), // Empty.
		Items<std::string> { "abc" }, // Single element.
		Items<std::string> { "a", "b", "c" }, // Sorted (odd count).
		Items<std::string> { "a", "b", "c", "d" }, // Sorted (even count).
		Items<std::string> { "f", "d", "a" }, // Sorted in reverce order (odd count).
		Items<std::string> { "z", "y", "x", "w" }, // Sorted in reverce order (even count).
		Items<std::string> { "asf", "aaecsads", "ds", "bf" }, // Arbitrary sequence (odd count).
		Items<std::string> { "asdfjoa", "vucoueb", "qepjnen", "csd", "eqcqxae" }, // Arbitrary sequence (even count).
	};
}

// Simplest workaround for implementation of test, 
// which are parametrized and type-parametrized in the same time.
template<typename T>
class SequencesFixture : public testing::Test {
public:
	std::vector<std::vector<T>> params;

protected:
	SequencesFixture()
		: params(CreateParams<T>())
	{}
};

TYPED_TEST_CASE_P(SequencesFixture);

TYPED_TEST_P(SequencesFixture, InsertionSort_ArbitrarySequence_SortedSequence) {
	for (auto items : params) {
		SortAlgorithms::InsertionSort(items.begin(), items.end(), std::less<TypeParam>());

		EXPECT_TRUE(std::is_sorted(items.begin(), items.end()));
	}
}

TYPED_TEST_P(SequencesFixture, QuckSort_ArbiturarySequence_SortedSequence) {
	for (auto items : params) {
		SortAlgorithms::QuickSort(items.begin(), items.end(), std::less<TypeParam>());

		EXPECT_TRUE(std::is_sorted(items.begin(), items.end()));
	}
}

TYPED_TEST_P(SequencesFixture, OptimizedQuckSort_ArbiturarySequence_SortedSequence) {
	for (auto items : params) {
		SortAlgorithms::OptimizedQuickSort(items.begin(), items.end(), std::less<TypeParam>());

		EXPECT_TRUE(std::is_sorted(items.begin(), items.end()));
	}
}

REGISTER_TYPED_TEST_CASE_P(
	SequencesFixture,
	InsertionSort_ArbitrarySequence_SortedSequence,
	QuckSort_ArbiturarySequence_SortedSequence,
	OptimizedQuckSort_ArbiturarySequence_SortedSequence);

typedef Types<int, std::string> Implementations;
INSTANTIATE_TYPED_TEST_CASE_P(SortAlgorithmsTests, SequencesFixture, Implementations);

}
