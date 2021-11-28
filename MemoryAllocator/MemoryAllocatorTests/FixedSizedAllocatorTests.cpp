#include <algorithm>
#include "pch.h"
#include "../MemoryAllocatorsLibrary/FixedSizedAllocator.h"

template<size_t block_size>
void AllocAndFill(fixed::FixedSizedAllocator<block_size>* FSA, std::vector<int*>* allocated, size_t count);

template<size_t block_size>
void Free(fixed::FixedSizedAllocator<block_size>* FSA, std::vector<int*>* allocated);

template<typename T>
bool ArePointersSequential(const std::vector<T*>& allocated);

template<typename T>
bool IsBelongToOnePage(const std::vector<T*> allocated);

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
	std::vector<int*> allocated;

	FSA.Init();

	AllocAndFill(&FSA, &allocated, blocks_count);

	EXPECT_TRUE(ArePointersSequential(allocated));
	
	Free(&FSA, &allocated);
	FSA.Destroy();
}

TEST(FixedSizedAllocator, Alloc_AllocMoreThenPage_MemoryAllocated) {
	const size_t blocks_count = 4;
	fixed::FixedSizedAllocator<blocks_count> FSA(blocks_count);
	std::vector<int*> allocated1;
	std::vector<int*> allocated2;

	FSA.Init();

	AllocAndFill(&FSA, &allocated1, blocks_count);
	AllocAndFill(&FSA, &allocated2, blocks_count);

	EXPECT_TRUE(ArePointersSequential(allocated1));
	EXPECT_TRUE(ArePointersSequential(allocated2));

	Free(&FSA, &allocated1);
	Free(&FSA, &allocated2);
	FSA.Destroy();
}

TEST(FixedSizedAllocator, Alloc_AllocFreeAlloc_AllPointersBelongToOnePage) {
	const size_t blocks_count = 4;
	fixed::FixedSizedAllocator<blocks_count> FSA(blocks_count);
	std::vector<int*> allocated;

	FSA.Init();

	AllocAndFill(&FSA, &allocated, blocks_count);
	Free(&FSA, &allocated);
	AllocAndFill(&FSA, &allocated, blocks_count);

	EXPECT_TRUE(IsBelongToOnePage(allocated));

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
	allocated[2] = (int*) FSA.Alloc();
	FSA.Free(allocated[3]);
	allocated[3] = (int*)FSA.Alloc();
	allocated[1] = (int*)FSA.Alloc();

	EXPECT_TRUE(IsBelongToOnePage(allocated));

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
bool IsBelongToOnePage(std::vector<T*> allocated) {
	std::sort(allocated.begin(), allocated.end());
	T* temp = allocated[0];

	for (size_t i = 0; i < allocated.size(); ++i) {
		if (temp != allocated[i]) {
			return false;
		}

		++temp;
	}

	return true;
}