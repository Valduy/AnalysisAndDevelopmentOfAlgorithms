#include "pch.h"
#include "../MemoryAllocatorsLibrary/FixedSizedAllocator.h"

TEST(FixedSizedAllocator, Alloc_AllocLessThenPage_MemoryAllocated) {
	const size_t blocks_count = 4;
	FixedSizedAllocator FSA(sizeof(int), 4);
	int* blocks[blocks_count];
	auto is_valid = [&]() {
		int* temp = blocks[0];

		for (size_t i = 0; i < blocks_count; ++i) {
			if (temp != blocks[i]) {
				return false;
			}

			++temp;
		}

		return true;
	};

	FSA.Init();

	for (size_t i = 0; i < blocks_count; ++i) {
		blocks[i] = (int*)FSA.Alloc();
	}

	EXPECT_TRUE(is_valid());
	
	for (size_t i = 0; i < blocks_count; ++i) {
		FSA.Free(blocks[i]);
	}

	FSA.Destroy();
}