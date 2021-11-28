#include "pch.h"
#include "../MemoryAllocatorsLibrary/MemoryAllocator.h"
#include "Util.h"

namespace manager_tests {

bool IsAllAllocated(const std::vector<void*>& allocated);

void Free(manager::MemoryAllocator* MA, std::vector<void*>* allocated);

TEST(MemoryAllocator, Alloc_AllocDifferentSizes_MemoryAllocated) {
	manager::MemoryAllocator MA;
	std::vector<void*> allocated;
	MA.Init();

	allocated.push_back(MA.Alloc(sizeof(TestFatClass<16>)));
	allocated.push_back(MA.Alloc(sizeof(TestFatClass<32>)));
	allocated.push_back(MA.Alloc(sizeof(TestFatClass<64>)));
	allocated.push_back(MA.Alloc(sizeof(TestFatClass<128>)));
	allocated.push_back(MA.Alloc(sizeof(TestFatClass<256>)));
	allocated.push_back(MA.Alloc(sizeof(TestFatClass<512>)));
	allocated.push_back(MA.Alloc(sizeof(TestFatClass<manager::kMaxCoalesceSize - coalesce::kBlockOffset>)));
	allocated.push_back(MA.Alloc(sizeof(TestFatClass<manager::kMaxCoalesceSize * 2>)));

	EXPECT_TRUE(IsAllAllocated(allocated));

	Free(&MA, &allocated);
	MA.Destroy();
}

bool IsAllAllocated(const std::vector<void*>& allocated) {
	return std::all_of(allocated.begin(), allocated.end(),
		[&](const void* p) {return p != nullptr; });
}

void Free(manager::MemoryAllocator* MA, std::vector<void*>* allocated) {
	for (void* p : *allocated) {
		MA->Free(p);
	}

	allocated->clear();
}

} // namespace manager_tests