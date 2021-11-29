#include <Windows.h>
#include <iostream>
#include "../MemoryAllocatorsLibrary/FixedSizedAllocator.h"
#include "../MemoryAllocatorsLibrary/CoalesceAllocator.h"

int main() {
	fixed::FixedSizedAllocator<sizeof(double)> FSA(5);
	FSA.Init();
	double* memory = (double*)FSA.Alloc();
	FSA.DumbStat();
	FSA.DumbBlocks();
	FSA.Free(memory);
	FSA.Destroy();

	return 0;
}