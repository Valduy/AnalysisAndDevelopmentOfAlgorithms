#include <algorithm>
#include <cmath>
#include "pch.h"
#include "../Array/Array.h"

class SequencesFixture : public testing::TestWithParam<std::vector<int>> {};

template<typename T>
void FillInDirectOrder(Array<T>& arr, const std::vector<T>& seq);

template<typename T>
void FillInReverceOrder(Array<T>& arr, const std::vector<T>& seq);

template<typename T>
void FillInCocktailOrder(Array<T>& arr, const std::vector<T>& seq);

template<typename T>
bool IsEqual(Array<T>& arr, const std::vector<T>& seq);

TEST(ArrayTests, Size_EmptyArray_SizeEqual0) {
	Array<int> arr;

	EXPECT_EQ(arr.Size(), 0);
}

TEST_P(SequencesFixture, Size_AddSomeElements_SizeEqualElementsCount) {
	auto seq = GetParam();
	Array<int> arr;

	FillInDirectOrder(arr, seq);

	EXPECT_EQ(arr.Size(), seq.size());
}

TEST_P(SequencesFixture, Insert_InsertElementsInDirectOrder_ElementsSequentiallyPresentedInArray) {
	auto seq = GetParam();	
	Array<int> arr;

	FillInDirectOrder(arr, seq);

	EXPECT_TRUE(IsEqual(arr, seq));
}

TEST_P(SequencesFixture, Insert_InsertElementsInReversOrder_ElementsSequentiallyPresentedInArray) {
	auto seq = GetParam();
	Array<int> arr;

	FillInReverceOrder(arr, seq);

	EXPECT_TRUE(IsEqual(arr, seq));
}

TEST_P(SequencesFixture, Insert_InsertElementsInCocktailOrder_ElementsSequentiallyPresentedInArray) {
	auto seq = GetParam();
	Array<int> arr;

	FillInCocktailOrder(arr, seq);

	EXPECT_TRUE(IsEqual(arr, seq));
}

TEST_P(SequencesFixture, Remove_RemoveLastElement_PresentedAllElementsExceptLast) {
	auto seq = GetParam();
	Array<int> arr;

	FillInDirectOrder(arr, seq);
	arr.Remove(arr.Size() - 1);
	seq.pop_back();

	EXPECT_TRUE(IsEqual(arr, seq));
}

TEST_P(SequencesFixture, Remove_RemoveFirstElement_PresentedAllElementsExceptFirst) {
	auto seq = GetParam();
	Array<int> arr;

	FillInDirectOrder(arr, seq);
	arr.Remove(0);
	seq.erase(seq.begin());

	EXPECT_TRUE(IsEqual(arr, seq));
}

TEST_P(SequencesFixture, Remove_RemoveMiddleElement_PresentedAllElementsExceptMiddle) {
	auto seq = GetParam();
	Array<int> arr;

	FillInDirectOrder(arr, seq);
	arr.Remove(arr.Size() / 2);
	seq.erase(seq.begin() + seq.size() / 2);

	EXPECT_TRUE(IsEqual(arr, seq));
}

TEST(ArrayTests, GetIterator_EnumerateEmptyArrayInDirectOrder_ArrayNotEnumerated) {
	Array<int> arr;
	auto is_valid = [&]() {
		for (auto arr_it = arr.GetIterator(); arr_it.HasCurrent(); arr_it.Next()) {
			return false;
		}

		return true;
	};

	EXPECT_TRUE(is_valid());
}

TEST_P(SequencesFixture, GetIterator_EnumerateArrayInDirectOrder_ElementsEnumeratedInDirectOrder) {
	auto seq = GetParam();
	Array<int> arr;
	auto is_valid = [&]() {
		auto seq_it = seq.begin();

		for (Array<int>::ConstIterator arr_it = arr.GetIterator(); arr_it.HasCurrent(); arr_it.Next()) {
			if (arr_it.Get() != *seq_it) {
				return false;
			}

			++seq_it;
		}

		return seq_it == seq.end();
	};
	
	FillInDirectOrder(arr, seq);

	EXPECT_TRUE(is_valid());
}

TEST_P(SequencesFixture, GetIterator_EnumerateAndSetValues_AllSetValuesPresentedInCorrectOrder) {
	auto seq = GetParam();
	Array<int> arr;
	
	for (int i = 0; i < seq.size(); ++i) {
		arr.Insert(0);
	}

	auto seq_it = seq.begin();

	for (Array<int>::Iterator arr_it = arr.GetIterator(); arr_it.HasCurrent(); arr_it.Next()) {
		arr_it.Set(*seq_it);
		++seq_it;
	}

	EXPECT_TRUE(IsEqual(arr, seq));
}

TEST(ArrayTests, GetReverceIterator_EnumerateEmptyArrayInReverceOrder_ArrayNotEnumerated) {
	Array<int> arr;
	auto is_valid = [&]() {
		for (auto arr_it = arr.GetReverceIterator(); arr_it.HasCurrent(); arr_it.Next()) {
			return false;
		}

		return true;
	};

	EXPECT_TRUE(is_valid());
}

TEST_P(SequencesFixture, GetReverceIterator_EnumerateArrayInReverceOrder_ElementsEnumeratedInReverceOrder) {
	auto seq = GetParam();
	Array<int> arr;
	auto is_valid = [&]() {
		auto seq_rit = seq.rbegin();

		for (Array<int>::ConstIterator arr_rit = arr.GetReverceIterator(); arr_rit.HasCurrent(); arr_rit.Next()) {
			if (arr_rit.Get() != *seq_rit) {
				return false;
			}

			++seq_rit;
		}

		return seq_rit == seq.rend();
	};

	FillInDirectOrder(arr, seq);

	EXPECT_TRUE(is_valid());
}

TEST_P(SequencesFixture, GetReverceIterator_EnumerateAndSetValues_AllSetValuesPresentedInCorrectOrder) {
	auto seq = GetParam();
	Array<int> arr;

	for (int i = 0; i < seq.size(); ++i) {
		arr.Insert(0);
	}

	auto seq_rit = seq.rbegin();

	for (Array<int>::Iterator arr_rit = arr.GetReverceIterator(); arr_rit.HasCurrent(); arr_rit.Next()) {
		arr_rit.Set(*seq_rit);
		++seq_rit;
	}

	EXPECT_TRUE(IsEqual(arr, seq));
}

TEST_P(SequencesFixture, ConstantIteration_IterateAllItems) {
	auto seq = GetParam();
	Array<int> arr;
	auto is_valid = [&]() {
		int i = 0;

		for (auto item : arr) {
			if (item != seq[i]) {
				return false;
			}

			++i;
		}

		return true;
	};

	FillInDirectOrder(arr, seq);

	EXPECT_TRUE(is_valid());
}

TEST_P(SequencesFixture, MutableIteration_IterateAndMutateAllItems) {
	auto seq = GetParam();
	Array<int> arr;

	FillInDirectOrder(arr, seq);

	for (auto it = arr.begin(); it != arr.end(); ++it) {
		*it += 1;
	}

	for (auto it = seq.begin(); it != seq.end(); ++it) {
		*it += 1;
	}

	EXPECT_TRUE(IsEqual(arr, seq));
}

INSTANTIATE_TEST_CASE_P(
	ArrayTests,
	SequencesFixture,
	testing::Values(
		std::vector<int> { 1 },
		std::vector<int> { 1, 2, 3, 4, 5 },
		std::vector<int> { 8, 3, 6, 7, 5, 9, 3 }
	),
);

template<typename T>
void FillInDirectOrder(Array<T>& arr, const std::vector<T>& seq) {
	for (int num : seq) {
		arr.Insert(num);
	}
}

template<typename T>
void FillInReverceOrder(Array<T>& arr, const std::vector<T>& seq) {
	for (int i = seq.size() - 1; i >= 0; --i) {
		arr.Insert(0, seq[i]);
	}
}

template<typename T>
void FillInCocktailOrder(Array<T>& arr, const std::vector<T>& seq) {
	int current_index = seq.size() - 1;
	int current_direction = -1;
	int next_index = 0;
	int next_direction = 1;

	while (arr.Size() != seq.size()) {
		int index = static_cast<int>(std::floor(arr.Size() / 2));
		arr.Insert(index, seq[current_index]);
		current_index += current_direction;
		std::swap(current_index, next_index);
		std::swap(current_direction, next_direction);		
	}
}

template<typename T>
bool IsEqual(Array<T>& arr, const std::vector<T>& seq) {
	for (int i = 0; i < seq.size(); ++i) {
		if (seq[i] != arr[i]) {
			return false;
		}			
	}

	return true;
}