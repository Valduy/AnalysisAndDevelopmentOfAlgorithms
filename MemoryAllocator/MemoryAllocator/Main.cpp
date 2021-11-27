#include <Windows.h>
#include <iostream>
#include "../MemoryAllocatorsLibrary/FixedSizedAllocator.h"
#include "../MemoryAllocatorsLibrary/CoalesceAllocator.h"

int main() {
	fixed::FixedSizedAllocator<sizeof(double)> FSA(5);
	FSA.Init();
	double* memory = (double*)FSA.Alloc();
	*memory = 9.1;
	FSA.Free(memory);
	FSA.Destroy();

	//coalesce::CoalesceAllocator<512> CA();
	std::cout << sizeof(coalesce::Block) << "\n";

	return 0;
}