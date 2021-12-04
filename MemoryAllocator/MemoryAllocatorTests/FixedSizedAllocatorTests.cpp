#include <algorithm>
#include "pch.h"
#include "../MemoryAllocatorsLibrary/FixedSizedAllocator.h"

namespace fixed_tests {

class FixedSizedAllocatorFixture : public testing::TestWithParam<size_t>{};

template<size_t block_size>
void AllocAndFill(fixed::FixedSizedAllocator<block_size>* FSA, std::vector<int*>* allocated, size_t count);

template<size_t block_size>
void Free(fixed::FixedSizedAllocator<block_size>* FSA, std::vector<int*>* allocated);

template<typename T>
bool ArePointersSequential(const T first, size_t size);

template<typename T>
bool ArePointersSequential(const std::vector<T*>& allocated);

template<typename T>
bool ArePointersSequentialForPages(const std::vector<T*>& allocated, const size_t blocks_count, const size_t pages_count);

template<typename T>
bool IsBelongToOnePage(std::vector<T*>* allocated);

template<typename T>
bool IsBelongToSomePages(std::vector<T*>* allocated, const size_t blocks_count, const size_t pages_count);

TEST(FixedSizedAllocator, Alloc_AllocWhenBlocksPerPageIs0_ReturnNull) {
	fixed::FixedSizedAllocator<sizeof(int)> FSA(0);
	FSA.Init();

	void* p = FSA.Alloc();

	EXPECT_EQ(p, nullptr);

	FSA.Destroy();
}

TEST(FixedSizedAllocator, Alloc_AllocPage_AllocatedPointersSequentialForPage) {
	const size_t blocks_count = 4;
	fixed::FixedSizedAllocator<sizeof(int)> FSA(blocks_count);
	std::vector<int*> allocated;

	FSA.Init();

	AllocAndFill(&FSA, &allocated, blocks_count);

	EXPECT_TRUE(ArePointersSequential(allocated));

	Free(&FSA, &allocated);
	FSA.Destroy();
}

TEST_P(FixedSizedAllocatorFixture, Alloc_AllocSomePages_AllocatedPointersSequentialForPages) {
	const size_t blocks_count = 4;
	const size_t pages_count = GetParam();
	fixed::FixedSizedAllocator<sizeof(int)> FSA(blocks_count);
	std::vector<int*> allocated;

	FSA.Init();

	AllocAndFill(&FSA, &allocated, blocks_count * pages_count);

	EXPECT_TRUE(ArePointersSequentialForPages(allocated, blocks_count, pages_count));

	Free(&FSA, &allocated);
	FSA.Destroy();
}

TEST_P(FixedSizedAllocatorFixture, Alloc_AllocSomePages_AllocatedPointersNotSequential) {
	const size_t blocks_count = 4;
	const size_t pages_count = GetParam();
	fixed::FixedSizedAllocator<sizeof(int)> FSA(blocks_count);
	std::vector<int*> allocated;

	FSA.Init();

	AllocAndFill(&FSA, &allocated, blocks_count * pages_count);

	EXPECT_FALSE(ArePointersSequential(allocated));

	Free(&FSA, &allocated);
	FSA.Destroy();
}

TEST(FixedSizedAllocator, Alloc_AllocFreeAllocPage_AllocatedPointersBelongToOnePage) {
	const size_t blocks_count = 4;
	fixed::FixedSizedAllocator<sizeof(int)> FSA(blocks_count);
	std::vector<int*> allocated;

	FSA.Init();

	AllocAndFill(&FSA, &allocated, blocks_count);
	Free(&FSA, &allocated);
	AllocAndFill(&FSA, &allocated, blocks_count);

	EXPECT_TRUE(IsBelongToOnePage(&allocated));

	Free(&FSA, &allocated);
	FSA.Destroy();
}

TEST_P(FixedSizedAllocatorFixture, Alloc_AllocFreeAllocSomePages_AllocatedPointersBelongToCorrespondingPages) {
	const size_t blocks_count = 4;
	const size_t pages_count = GetParam();
	fixed::FixedSizedAllocator<sizeof(int)> FSA(blocks_count);
	std::vector<int*> allocated;

	FSA.Init();

	AllocAndFill(&FSA, &allocated, blocks_count * pages_count);
	Free(&FSA, &allocated);
	AllocAndFill(&FSA, &allocated, blocks_count * pages_count);

	EXPECT_TRUE(IsBelongToSomePages(&allocated, blocks_count, pages_count));

	Free(&FSA, &allocated);
	FSA.Destroy();
}

TEST_P(FixedSizedAllocatorFixture, Alloc_AllocFreeAllocSomePages_AllocatedPointersNotBelongToOnePage) {
	const size_t blocks_count = 4;
	const size_t pages_count = GetParam();
	fixed::FixedSizedAllocator<sizeof(int)> FSA(blocks_count);
	std::vector<int*> allocated;

	FSA.Init();

	AllocAndFill(&FSA, &allocated, blocks_count * pages_count);
	Free(&FSA, &allocated);
	AllocAndFill(&FSA, &allocated, blocks_count * pages_count);

	EXPECT_FALSE(IsBelongToOnePage(&allocated));

	Free(&FSA, &allocated);
	FSA.Destroy();
}

TEST(FixedSizedAllocator, Alloc_AllocAndFreeMemoryOfOnePage_AllPointersBelongToOnePage) {
	const size_t blocks_count = 4;
	fixed::FixedSizedAllocator<blocks_count> FSA(blocks_count);
	std::vector<int*> allocated;

	FSA.Init();

	AllocAndFill(&FSA, &allocated, blocks_count);
	FSA.Free(allocated[2]);
	FSA.Free(allocated[1]);
	allocated[2] = (int*)FSA.Alloc();
	FSA.Free(allocated[3]);
	allocated[3] = (int*)FSA.Alloc();
	allocated[1] = (int*)FSA.Alloc();

	EXPECT_TRUE(IsBelongToOnePage(&allocated));

	Free(&FSA, &allocated);
	FSA.Destroy();
}

template<size_t block_size>
void AllocAndFill(fixed::FixedSizedAllocator<block_size>* FSA, std::vector<int*>* allocated, size_t count) {
	for (size_t i = 0; i < count; ++i) {
		int* num = (int*)FSA->Alloc();
		*num = i;
		allocated->push_back(num);
	}
}

template<size_t block_size>
void Free(fixed::FixedSizedAllocator<block_size>* FSA, std::vector<int*>* allocated) {
	for (void* p : *allocated) {
		FSA->Free(p);
	}

	allocated->clear();
}

template<typename T>
bool ArePointersSequential(const T first, size_t size) {
	T temp = first;

	for (size_t i = 0; i < size; ++i) {
		if (temp != first + i) {
			return false;
		}

		++temp;
	}

	return true;
}

template<typename T>
bool ArePointersSequential(const std::vector<T*>& allocated) {
	T* temp = allocated[0];

	for (size_t i = 0; i < allocated.size(); ++i) {
		if (temp != allocated[i]) {
			return false;
		}

		++temp;
	}

	return true;
}

template<typename T>
bool ArePointersSequentialForPages(const std::vector<T*>& allocated, const size_t blocks_count, const size_t pages_count) {
	for (size_t i = 0; i < pages_count; ++i) {
		auto first = std::next(allocated.begin(), blocks_count * i);

		if (!ArePointersSequential(first, blocks_count)) {
			return false;
		}
	}

	return true;
}

template<typename T>
bool IsBelongToOnePage(std::vector<T*>* allocated) {
	std::sort(allocated->begin(), allocated->end());
	return ArePointersSequential(*allocated);
}

template<typename T>
bool IsBelongToSomePages(std::vector<T*>* allocated, const size_t blocks_count, const size_t pages_count) {
	for (size_t i = 0; i < pages_count; ++i) {
		auto first = std::next(allocated->begin(), blocks_count * i);
		auto last = std::next(allocated->begin(), blocks_count * (i + 1));
		std::vector<T*> piece;
		piece.insert(piece.begin(), first, last);

		if (!IsBelongToOnePage(&piece)) {
			return false;
		}
	}

	return true;
}

INSTANTIATE_TEST_CASE_P(
	FixedSizedAllocatorTests,
	FixedSizedAllocatorFixture,
	testing::Values(
		2, 3, 4
	),);

} // namespace fixed_tests

