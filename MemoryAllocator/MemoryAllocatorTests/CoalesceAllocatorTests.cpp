#include <vector>
#include "pch.h"
#include "../MemoryAllocatorsLibrary/CoalesceAllocator.h"
#include "Util.h"

namespace coalesce_tests {

bool IsAllAllocated(const std::vector<void*>& allocated);

template<size_t block_size>
void AllocAndFill(coalesce::CoalesceAllocator<block_size>* CA, std::vector<void*>* allocated, size_t count);

template<size_t buffer_size>
void Free(coalesce::CoalesceAllocator<buffer_size>* CA, std::vector<void*>* allocated);

TEST(CoalesceAllocator, Alloc_AllocLessThenPage_MemoryAllocated) {
	const size_t count = 4;
	const size_t buffer_size = sizeof(coalesce::Block) * count;
	coalesce::CoalesceAllocator<buffer_size> CA;
	std::vector<void*> allocated;
	CA.Init();

	AllocAndFill(&CA, &allocated, count);

	EXPECT_TRUE(IsAllAllocated(allocated));

	Free(&CA, &allocated);
	CA.Destroy();
}

TEST(CoalesceAllocator, Alloc_AllocMoreThenPage_MemoryAllocated) {
	const size_t count = 8;
	const size_t buffer_size = sizeof(coalesce::Block) * count / 2;
	coalesce::CoalesceAllocator<buffer_size> CA;
	std::vector<void*> allocated;
	CA.Init();

	AllocAndFill(&CA, &allocated, count);

	EXPECT_TRUE(IsAllAllocated(allocated));

	Free(&CA, &allocated);
	CA.Destroy();
}

TEST(CoalesceAllocator, Alloc_AllocFreeAlloc_MemoryAllocated) {
	const size_t buffer_size = sizeof(coalesce::Block) * 4;
	coalesce::CoalesceAllocator<buffer_size> CA;
	std::vector<void*> allocated;
	CA.Init();

	AllocAndFill(&CA, &allocated, 4);
	Free(&CA, &allocated);
	AllocAndFill(&CA, &allocated, 4);

	EXPECT_TRUE(IsAllAllocated(allocated));

	Free(&CA, &allocated);
	CA.Destroy();
}

TEST(CoalesceAllocator, Alloc_AllocAndFreeMemoryOfOnePage_MemoryAllocated) {
	const size_t buffer_size = sizeof(coalesce::Block) * 4;
	coalesce::CoalesceAllocator<buffer_size> CA;
	std::vector<void*> allocated;
	CA.Init();

	AllocAndFill(&CA, &allocated, 4);
	CA.Free(allocated[2]);
	CA.Free(allocated[1]);
	allocated[2] = (int*)CA.Alloc(sizeof(int));
	CA.Free(allocated[3]);
	allocated[3] = (int*)CA.Alloc(sizeof(int));
	allocated[1] = (int*)CA.Alloc(sizeof(int));

	EXPECT_TRUE(IsAllAllocated(allocated));

	Free(&CA, &allocated);
	CA.Destroy();
}

TEST(CoalesceAllocator, Alloc_AllocDifferentSizes_MemoryAllocated) {
	const size_t buffer_size = coalesce::kBlockOffset + sizeof(TestFatClass<256>);
	coalesce::CoalesceAllocator<buffer_size> CA;
	std::vector<void*> allocated;
	CA.Init();

	allocated.push_back(CA.Alloc(sizeof(TestFatClass<64>)));
	allocated.push_back(CA.Alloc(sizeof(TestFatClass<128>)));
	allocated.push_back(CA.Alloc(sizeof(TestFatClass<256>)));

	EXPECT_TRUE(IsAllAllocated(allocated));

	Free(&CA, &allocated);
	CA.Destroy();
}

bool IsAllAllocated(const std::vector<void*>& allocated) {
	return std::all_of(allocated.begin(), allocated.end(),
		[&](const void* p) {return p != nullptr; });
}

template<size_t block_size>
void AllocAndFill(coalesce::CoalesceAllocator<block_size>* CA, std::vector<void*>* allocated, size_t count) {
	for (size_t i = 0; i < count; ++i) {
		int* num = (int*)CA->Alloc(sizeof(int));
		*num = i;
		allocated->push_back(num);
	}
}

template<size_t buffer_size>
void Free(coalesce::CoalesceAllocator<buffer_size>* CA, std::vector<void*>* allocated) {
	for (void* p : *allocated) {
		CA->Free(p);
	}

	allocated->clear();
}

} // coalesce_tests

