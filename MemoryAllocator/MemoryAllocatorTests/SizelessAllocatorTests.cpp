#include <vector>
#include "pch.h"
#include "../MemoryAllocatorsLibrary/SizelessAllocator.h"
#include "Util.h"

namespace sizeless_tests {

bool IsAllAllocated(const std::vector<void*>& allocated);

void Free(sizeless::SizelessAllocator* SA, std::vector<void*>* allocated);

TEST(SizelessAllocator, Alloc_AllocSinglePage_MemoryAllocated) {
	sizeless::SizelessAllocator SA;

	void* p = SA.Alloc(sizeof(TestFatClass<16>));

	EXPECT_NE(p, nullptr);

	SA.Free(p);
}

TEST(SizelessAllocator, Alloc_AllocSomePages_MemoryAllocated) {
	sizeless::SizelessAllocator SA;
	std::vector<void*> allocated;

	allocated.push_back(SA.Alloc(sizeof(TestFatClass<16>)));
	allocated.push_back(SA.Alloc(sizeof(TestFatClass<32>)));
	allocated.push_back(SA.Alloc(sizeof(TestFatClass<64>)));

	EXPECT_TRUE(IsAllAllocated(allocated));

	Free(&SA, &allocated);
}

bool IsAllAllocated(const std::vector<void*>& allocated) {
	return std::all_of(allocated.begin(), allocated.end(),
		[&](const void* p) {return p != nullptr; });
}

void Free(sizeless::SizelessAllocator* SA, std::vector<void*>* allocated) {
	for (void* p : *allocated) {
		SA->Free(p);
	}

	allocated->clear();
}

} // namespace sizeless_tests

