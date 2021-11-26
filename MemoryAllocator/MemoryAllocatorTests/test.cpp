#include <algorithm>
#include "pch.h"
#include "../MemoryAllocatorsLibrary/FixedSizedAllocator.h"

template<size_t block_size>
void AllocAndFill(fixed::FixedSizedAllocator<block_size>* FSA, int** pointers, size_t size);

template<size_t block_size>
void Free(fixed::FixedSizedAllocator<block_size>* FSA, int** pointers, size_t size);

template<typename T>
bool ArePointersSequential(T** pointers, size_t size);

template<typename T>
bool IsBelongToOnePage(T** pointers, size_t size);

TEST(FixedSizedAllocator, Alloc_AllocWhenBlocksPerPageIs0_ReturnNull) {
	fixed::FixedSizedAllocator<sizeof(int)> FSA(0);
	FSA.Init();

	void* p = FSA.Alloc();

	EXPECT_EQ(p, nullptr);

	FSA.Destroy();
}

TEST(FixedSizedAllocator, Alloc_AllocLessThenPage_MemoryAllocated) {
	const size_t blocks_count = 4;
	fixed::FixedSizedAllocator<blocks_count> FSA(blocks_count);
	int* blocks[blocks_count];

	FSA.Init();

	AllocAndFill(&FSA, blocks, blocks_count);

	EXPECT_TRUE(ArePointersSequential(blocks, blocks_count));
	
	Free(&FSA, blocks, blocks_count);
	FSA.Destroy();
}

TEST(FixedSizedAllocator, Alloc_AllocMoreThenPage_MemoryAllocated) {
	const size_t blocks_count = 4;
	fixed::FixedSizedAllocator<blocks_count> FSA(blocks_count);
	int* page1[blocks_count];
	int* page2[blocks_count];

	FSA.Init();

	AllocAndFill(&FSA, page1, blocks_count);
	AllocAndFill(&FSA, page2, blocks_count);

	EXPECT_TRUE(ArePointersSequential(page1, blocks_count));
	EXPECT_TRUE(ArePointersSequential(page2, blocks_count));

	Free(&FSA, page1, blocks_count);
	Free(&FSA, page2, blocks_count);
	FSA.Destroy();
}

TEST(FixedSizedAllocator, Alloc_AllocFreeAlloc_AllPointersBelongToOnePage) {
	const size_t blocks_count = 4;
	fixed::FixedSizedAllocator<blocks_count> FSA(blocks_count);
	int* blocks[blocks_count];

	FSA.Init();

	AllocAndFill(&FSA, blocks, blocks_count);
	Free(&FSA, blocks, blocks_count);
	AllocAndFill(&FSA, blocks, blocks_count);

	EXPECT_TRUE(IsBelongToOnePage(blocks, blocks_count));

	Free(&FSA, blocks, blocks_count);
	FSA.Destroy();
}

TEST(FixedSizedAllocator, Alloc_AllocAndFreeMemoryOfOnePage_AllPointersBelongToOnePage) {
	const size_t blocks_count = 4;
	fixed::FixedSizedAllocator<blocks_count> FSA(blocks_count);
	int* blocks[blocks_count];

	FSA.Init();

	AllocAndFill(&FSA, blocks, blocks_count);
	FSA.Free(blocks[2]);
	FSA.Free(blocks[1]);
	blocks[2] = (int*) FSA.Alloc();
	FSA.Free(blocks[3]);
	blocks[3] = (int*)FSA.Alloc();
	blocks[1] = (int*)FSA.Alloc();

	EXPECT_TRUE(IsBelongToOnePage(blocks, blocks_count));

	Free(&FSA, blocks, blocks_count);
	FSA.Destroy();
}

template<size_t block_size>
void AllocAndFill(fixed::FixedSizedAllocator<block_size>* FSA, int** pointers, size_t size) {
	for (size_t i = 0; i < size; ++i) {
		pointers[i] = (int*)(*FSA).Alloc();
		*pointers[i] = i;
	}
}

template<size_t block_size>
void Free(fixed::FixedSizedAllocator<block_size>* FSA, int** pointers, size_t size) {
	for (size_t i = 0; i < size; ++i) {
		(*FSA).Free(pointers[i]);
	}
}

template<typename T>
bool ArePointersSequential(T** pointers, size_t size) {
	int* temp = pointers[0];

	for (size_t i = 0; i < size; ++i) {
		if (temp != pointers[i]) {
			return false;
		}

		++temp;
	}

	return true;
}

template<typename T>
bool IsBelongToOnePage(T** pointers, size_t size) {
	std::sort(pointers, pointers + size);
	T* temp = pointers[0];

	for (size_t i = 0; i < size; ++i) {
		if (temp != pointers[i]) {
			return false;
		}

		++temp;
	}

	return true;
}