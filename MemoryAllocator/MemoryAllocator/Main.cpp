#include <deque>
#include <iostream>

#include "../MemoryAllocatorsLibrary/MemoryAllocator.h"

template<size_t size>
class FatClass {
private:
	char buffer_[size];
};

void Free(manager::MemoryAllocator* MA, std::deque<void*>* allocated) {
	for (void* p : *allocated) {
		MA->Free(p);
	}

	allocated->clear();
}

int main() {
	constexpr size_t cycles_count = 20;
	constexpr size_t allocations_per_cycle = 2;
	constexpr size_t free_per_cycle = 1;

	manager::MemoryAllocator MA;
	MA.Init();

	std::deque<void*> fixed16_allocated;
	std::deque<void*> fixed32_allocated;
	std::deque<void*> fixed64_allocated;
	std::deque<void*> fixed128_allocated;
	std::deque<void*> fixed256_allocated;
	std::deque<void*> fixed512_allocated;
	std::deque<void*> coalesce_allocated;
	std::deque<void*> sizeless_allocated;

	std::cout << "Perform " << cycles_count << " cycles.\n";
	std::cout << "Perform " << allocations_per_cycle << " allocations per cycle.\n";
	std::cout << "Perform " << free_per_cycle << " free per cycle.\n";

	for (size_t i = 0; i < cycles_count; ++i) {
		for (size_t j = 0; j < allocations_per_cycle; ++j) {
			fixed16_allocated.push_back(MA.Alloc(sizeof(FatClass<16>)));
			fixed32_allocated.push_back(MA.Alloc(sizeof(FatClass<32>)));
			fixed64_allocated.push_back(MA.Alloc(sizeof(FatClass<64>)));
			fixed128_allocated.push_back(MA.Alloc(sizeof(FatClass<128>)));
			fixed256_allocated.push_back(MA.Alloc(sizeof(FatClass<256>)));
			fixed512_allocated.push_back(MA.Alloc(sizeof(FatClass<512>)));
			coalesce_allocated.push_back(MA.Alloc(sizeof(FatClass<1024>)));
			sizeless_allocated.push_back(MA.Alloc(sizeof(FatClass<manager::kMaxCoalesceSize * 2>)));
		}

		for (size_t j = 0; j < free_per_cycle; ++j) {
			MA.Free(fixed16_allocated.front());
			fixed16_allocated.pop_front();
			MA.Free(fixed32_allocated.front());
			fixed32_allocated.pop_front();
			MA.Free(fixed64_allocated.front());
			fixed64_allocated.pop_front();
			MA.Free(fixed128_allocated.front());
			fixed128_allocated.pop_front();
			MA.Free(fixed256_allocated.front());
			fixed256_allocated.pop_front();
			MA.Free(fixed512_allocated.front());
			fixed512_allocated.pop_front();
			MA.Free(coalesce_allocated.front());
			coalesce_allocated.pop_front();
			MA.Free(sizeless_allocated.front());
			sizeless_allocated.pop_front();
		}

		std::cout << "Cycle " << i << " statistics:\n";
		MA.DumpStat();
		MA.DumpBlocks();
	}

	Free(&MA, &fixed16_allocated);
	Free(&MA, &fixed32_allocated);
	Free(&MA, &fixed64_allocated);
	Free(&MA, &fixed128_allocated);
	Free(&MA, &fixed256_allocated);
	Free(&MA, &fixed512_allocated);
	Free(&MA, &coalesce_allocated);
	Free(&MA, &sizeless_allocated);

	MA.Destroy();
	return 0;
}