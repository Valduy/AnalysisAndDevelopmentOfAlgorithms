#include "pch.h"
#include "../Quicksort/SortAlgorithms.h"

class SortAlgorithmsParametrizedTestFixture : public testing::TestWithParam<std::vector<int>> { };

TEST_P(SortAlgorithmsParametrizedTestFixture, InsertionSort_ArbitrarySequence_SortedSequence) {
	auto sequence = GetParam();

	SortAlgorithms::InsertionSort(sequence.begin(), sequence.end(), std::less<int>());

	EXPECT_TRUE(std::is_sorted(sequence.begin(), sequence.end()));
}

TEST_P(SortAlgorithmsParametrizedTestFixture, QuckSort_ArbiturarySequence_SortedSequence) {
	auto sequence = GetParam();

	SortAlgorithms::QuickSort(sequence.begin(), sequence.end(), std::less<int>());

	EXPECT_TRUE(std::is_sorted(sequence.begin(), sequence.end()));
}

INSTANTIATE_TEST_CASE_P(
	SortAlgorithmsTests,
	SortAlgorithmsParametrizedTestFixture,
	testing::Values(
		std::vector<int> (), // Empty.
		std::vector<int> { 4 }, // Single element.
		std::vector<int> { 1, 2, 3, 4, 5 }, // Sorted (odd count).
		std::vector<int> { 1, 2, 3, 4, 5, 6 }, // Sorted (even count).
		std::vector<int> { 7, 6, 5, 4, 3 }, // Sorted (odd count).
		std::vector<int> { 8, 7, 6, 5, 4, 3 }, // Sorted in reverce order (odd count).
		std::vector<int> { 8, 4, 3, 6, 2, 15, 10, 0, -5, 18, 101, 19, 29, 31, 31, -15, 75 }, // Arbitrary sequence (odd count).
		std::vector<int> { 3, 2, 7, 15, -8, 25, 1, 1, 6, 9, -3, 28, 73, -31, -8, 41 } // Arbitrary sequence (even count).
), );