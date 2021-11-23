#include "pch.h"
#include "../MemoryAllocatorsLibrary/FixedSizedAllocator.h"

template<size_t block_size>
void AllocAndFill(FixedSizedAllocator<block_size>* FSA, int** pointers, size_t size);

template<size_t block_size>
void Free(FixedSizedAllocator<block_size>* FSA, int** pointers, size_t size);

template<typename T>
bool ArePointersSequential(T** pointers, size_t size);

TEST(FixedSizedAllocator, Alloc_AllocWhenBlocksPerPageIs0_ReturnNull) {
	FixedSizedAllocator<sizeof(int)> FSA(0);
	FSA.Init();

	void* p = FSA.Alloc();

	EXPECT_EQ(p, nullptr);

	FSA.Destroy();
}

TEST(FixedSizedAllocator, Alloc_AllocLessThenPage_MemoryAllocated) {
	const size_t blocks_count = sizeof(int);
	FixedSizedAllocator<blocks_count> FSA(blocks_count);
	int* blocks[blocks_count];

	FSA.Init();

	AllocAndFill(&FSA, blocks, blocks_count);

	EXPECT_TRUE(ArePointersSequential(blocks, blocks_count));
	
	Free(&FSA, blocks, blocks_count);
	FSA.Destroy();
}

TEST(FixedSizedAllocator, Alloc_AllocMoreThenPage_MemoryAllocated) {
	const size_t blocks_count = sizeof(int);
	FixedSizedAllocator<blocks_count> FSA(blocks_count);
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

template<size_t block_size>
void AllocAndFill(FixedSizedAllocator<block_size>* FSA, int** pointers, size_t size) {
	for (size_t i = 0; i < size; ++i) {
		pointers[i] = (int*)(*FSA).Alloc();
		*pointers[i] = i;
	}
}

template<size_t block_size>
void Free(FixedSizedAllocator<block_size>* FSA, int** pointers, size_t size) {
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