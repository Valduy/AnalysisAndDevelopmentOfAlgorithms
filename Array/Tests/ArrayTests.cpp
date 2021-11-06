#include <algorithm>
#include <cmath>
#include <string>
#include "pch.h"
#include "../Array/Array.h"
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
		Items<int> { 1 },
		Items<int> { 1, 2, 3, 4, 5 },
		Items<int> { 8, 3, 6, 7, 5, 9, 3 } 
	};
}

template<>
Params<std::string> CreateParams() {
	return {
		Items<std::string> { "abc" },
		Items<std::string> { "abc", "def", "ghi" },
		Items<std::string> { "a", "bc", "def", "ghij", "klmno" },
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

template<typename T>
void FillInDirectOrder(Array<T>& arr, const Items<T>& items);

template<typename T>
void FillInReverceOrder(Array<T>& arr, const Items<T>& items);

template<typename T>
void FillInCocktailOrder(Array<T>& arr, const Items<T>& items);

template<typename T>
bool IsEqual(Array<T>& arr, const Items<T>& seq);

TYPED_TEST_P(SequencesFixture, Size_EmptyArray_SizeEqual0) {
	Array<TypeParam> arr;

	EXPECT_EQ(arr.Size(), 0);
}

TYPED_TEST_P(SequencesFixture, Size_AddSomeElements_SizeEqualElementsCount) {
	for (auto items : params) {
		Array<TypeParam> arr;

		FillInDirectOrder(arr, items);

		EXPECT_EQ(arr.Size(), items.size());
	}
}

TYPED_TEST_P(SequencesFixture, Insert_InsertElementsInDirectOrder_ElementsSequentiallyPresentedInArray) {
	for (auto items : params) {
		Array<TypeParam> arr;

		FillInDirectOrder(arr, items);

		EXPECT_TRUE(IsEqual(arr, items));
	}
}

TYPED_TEST_P(SequencesFixture, Insert_InsertElementsInReversOrder_ElementsReverslyPresentedInArray) {
	for (auto items : params) {
		Array<TypeParam> arr;

		FillInReverceOrder(arr, items);

		EXPECT_TRUE(IsEqual(arr, items));
	}
}

TYPED_TEST_P(SequencesFixture, Insert_InsertElementsInCocktailOrder_ElementsCocktaillyPresentedInArray) {
	for (auto items : params) {
		Array<TypeParam> arr;

		FillInCocktailOrder(arr, items);

		EXPECT_TRUE(IsEqual(arr, items));
	}
}

TYPED_TEST_P(SequencesFixture, Remove_RemoveLastElement_PresentedAllElementsExceptLast) {
	for (auto items : params) {
		Array<TypeParam> arr;

		FillInDirectOrder(arr, items);
		arr.Remove(arr.Size() - 1);
		items.pop_back();

		EXPECT_TRUE(IsEqual(arr, items));
	}
}

TYPED_TEST_P(SequencesFixture, Remove_RemoveFirstElement_PresentedAllElementsExceptFirst) {
	for (auto items : params) {
		Array<TypeParam> arr;

		FillInDirectOrder(arr, items);
		arr.Remove(0);
		items.erase(items.begin());

		EXPECT_TRUE(IsEqual(arr, items));
	}
}

TYPED_TEST_P(SequencesFixture, Remove_RemoveMiddleElement_PresentedAllElementsExceptMiddle) {
	for (auto items : params) {
		Array<TypeParam> arr;

		FillInDirectOrder(arr, items);
		arr.Remove(arr.Size() / 2);
		items.erase(items.begin() + items.size() / 2);

		EXPECT_TRUE(IsEqual(arr, items));
	}
}

TYPED_TEST_P(SequencesFixture, GetIterator_EnumerateEmptyArrayInDirectOrder_ArrayNotEnumerated) {
	Array<TypeParam> arr;
	auto is_valid = [&]() {
		for (auto arr_it = arr.GetIterator(); arr_it.HasCurrent(); arr_it.Next()) {
			return false;
		}

		return true;
	};

	EXPECT_TRUE(is_valid());
}

TYPED_TEST_P(SequencesFixture, GetIterator_EnumerateArrayInDirectOrder_ElementsEnumeratedInDirectOrder) {
	for (auto items : params) {
		Array<TypeParam> arr;
		auto is_valid = [&]() {
			auto items_it = items.begin();

			for (Array<TypeParam>::ConstIterator arr_it = arr.GetIterator(); arr_it.HasCurrent(); arr_it.Next()) {
				if (arr_it.Get() != *items_it) {
					return false;
				}

				++items_it;
			}

			return items_it == items.end();
		};

		FillInDirectOrder(arr, items);

		EXPECT_TRUE(is_valid());
	}
}

TYPED_TEST_P(SequencesFixture, GetIterator_EnumerateAndSetValues_AllSetValuesPresentedInCorrectOrder) {
	for (auto items : params) {
		Array<TypeParam> arr;

		for (int i = 0; i < items.size(); ++i) {
			arr.Insert(TypeParam());
		}

		auto items_it = items.begin();

		for (Array<TypeParam>::Iterator arr_it = arr.GetIterator(); arr_it.HasCurrent(); arr_it.Next()) {
			arr_it.Set(*items_it);
			++items_it;
		}

		EXPECT_TRUE(IsEqual(arr, items));
	}
}

TYPED_TEST_P(SequencesFixture, GetReverceIterator_EnumerateEmptyArrayInReverceOrder_ArrayNotEnumerated) {
	Array<TypeParam> arr;
	auto is_valid = [&]() {
		for (auto arr_it = arr.GetReverceIterator(); arr_it.HasCurrent(); arr_it.Next()) {
			return false;
		}

		return true;
	};

	EXPECT_TRUE(is_valid());
}

TYPED_TEST_P(SequencesFixture, GetReverceIterator_EnumerateArrayInReverceOrder_ElementsEnumeratedInReverceOrder) {
	for (auto items : params) {
		Array<TypeParam> arr;
		auto is_valid = [&]() {
			auto items_rit = items.rbegin();

			for (Array<TypeParam>::ConstIterator arr_rit = arr.GetReverceIterator(); arr_rit.HasCurrent(); arr_rit.Next()) {
				if (arr_rit.Get() != *items_rit) {
					return false;
				}

				++items_rit;
			}

			return items_rit == items.rend();
		};

		FillInDirectOrder(arr, items);

		EXPECT_TRUE(is_valid());
	}
}

TYPED_TEST_P(SequencesFixture, GetReverceIterator_EnumerateAndSetValues_AllSetValuesPresentedInCorrectOrder) {
	for (auto items : params) {
		Array<TypeParam> arr;

		for (int i = 0; i < items.size(); ++i) {
			arr.Insert(TypeParam());
		}

		auto items_rit = items.rbegin();

		for (Array<TypeParam>::Iterator arr_rit = arr.GetReverceIterator(); arr_rit.HasCurrent(); arr_rit.Next()) {
			arr_rit.Set(*items_rit);
++items_rit;
		}

		EXPECT_TRUE(IsEqual(arr, items));
	}

}

TYPED_TEST_P(SequencesFixture, ConstantIteration_IterateAllItems) {
	for (auto items : params) {
		Array<TypeParam> arr;
		auto is_valid = [&]() {
			int i = 0;

			for (auto item : arr) {
				if (item != items[i]) {
					return false;
				}

				++i;
			}

			return true;
		};

		FillInDirectOrder(arr, items);

		EXPECT_TRUE(is_valid());
	}
}

TYPED_TEST_P(SequencesFixture, MutableIteration_IterateAndMutateAllItems) {
	for (auto items : params) {
		Array<TypeParam> arr;

		FillInDirectOrder(arr, items);

		for (auto it = arr.begin(); it != arr.end(); ++it) {
			Mutate(*it);
		}

		for (auto it = items.begin(); it != items.end(); ++it) {
			Mutate(*it);
		}

		EXPECT_TRUE(IsEqual(arr, items));
	}
}

template<typename T>
void FillInDirectOrder(Array<T>& arr, const Items<T>& items) {
	for (T num : items) {
		arr.Insert(num);
	}
}

template<typename T>
void FillInReverceOrder(Array<T>& arr, const Items<T>& items) {
	for (int i = items.size() - 1; i >= 0; --i) {
		arr.Insert(0, items[i]);
	}
}

template<typename T>
void FillInCocktailOrder(Array<T>& arr, const std::vector<T>& items) {
	int current_index = items.size() - 1;
	int current_direction = -1;
	int next_index = 0;
	int next_direction = 1;

	while (arr.Size() != items.size()) {
		int index = static_cast<int>(std::floor(arr.Size() / 2));
		arr.Insert(index, items[current_index]);
		current_index += current_direction;
		std::swap(current_index, next_index);
		std::swap(current_direction, next_direction);
	}
}

template<typename T>
bool IsEqual(Array<T>& arr, const Items<T>& items) {
	for (int i = 0; i < items.size(); ++i) {
		if (items[i] != arr[i]) {
			return false;
		}
	}

	return true;
}

template<typename T>
void Mutate(T& value);

template<>
void Mutate<int>(int& value) {
	value /= value;
}

template<>
void Mutate(std::string& value){
	value += value;
}

REGISTER_TYPED_TEST_CASE_P(
	SequencesFixture,
	Size_EmptyArray_SizeEqual0,
	Size_AddSomeElements_SizeEqualElementsCount,
	Insert_InsertElementsInDirectOrder_ElementsSequentiallyPresentedInArray,
	Insert_InsertElementsInReversOrder_ElementsReverslyPresentedInArray,
	Insert_InsertElementsInCocktailOrder_ElementsCocktaillyPresentedInArray,
	Remove_RemoveLastElement_PresentedAllElementsExceptLast,
	Remove_RemoveFirstElement_PresentedAllElementsExceptFirst,
	Remove_RemoveMiddleElement_PresentedAllElementsExceptMiddle,
	GetIterator_EnumerateEmptyArrayInDirectOrder_ArrayNotEnumerated,
	GetIterator_EnumerateArrayInDirectOrder_ElementsEnumeratedInDirectOrder,
	GetIterator_EnumerateAndSetValues_AllSetValuesPresentedInCorrectOrder,
	GetReverceIterator_EnumerateEmptyArrayInReverceOrder_ArrayNotEnumerated,
	GetReverceIterator_EnumerateArrayInReverceOrder_ElementsEnumeratedInReverceOrder,
	GetReverceIterator_EnumerateAndSetValues_AllSetValuesPresentedInCorrectOrder,
	ConstantIteration_IterateAllItems,
	MutableIteration_IterateAndMutateAllItems);

typedef Types<int, std::string> Implementations;
INSTANTIATE_TYPED_TEST_CASE_P(ArrayTests, SequencesFixture, Implementations);
 
} // namespace