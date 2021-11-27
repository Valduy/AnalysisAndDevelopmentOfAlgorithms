#include <vector>
#include "pch.h"
#include "../MemoryAllocatorsLibrary/CoalesceAllocator.h"

template<size_t buffer_size>
void Free(coalesce::CoalesceAllocator<buffer_size>* CA, std::vector<void*> allocated);

TEST(CoalesceAllocator, Alloc_AllocLessThenBufferSize_MemoryAllocated) {
	const size_t buffer_size = sizeof(coalesce::Block) * 4;
	coalesce::CoalesceAllocator<buffer_size> CA;
	std::vector<void*> allocated;
	CA.Init();

	for (int i = 0; i < 4; ++i) {
		allocated.push_back(CA.Alloc(sizeof(int)));
	}

	Free(&CA, allocated);
	CA.Destroy();
}

template<size_t buffer_size>
void Free(coalesce::CoalesceAllocator<buffer_size>* CA, std::vector<void*> allocated) {
	for (void* p : allocated) {
		CA->Free(p);
	}
}