#include <Windows.h>
#include <iostream>
#include "../MemoryAllocatorsLibrary/FixedSizedAllocator.h"
#include "../MemoryAllocatorsLibrary/CoalesceAllocator.h"
#include "../MemoryAllocatorsLibrary/SizelessAllocator.h"

int main() {
	fixed::FixedSizedAllocator<sizeof(double)> FSA(5);
	FSA.Init();
	double* memory = (double*)FSA.Alloc();
	FSA.DumbStat();
	FSA.DumbBlocks();
	FSA.Free(memory);
	FSA.Destroy();

	coalesce::CoalesceAllocator<128> CA;
	CA.Init();
	memory = (double*)CA.Alloc(sizeof(double));
	CA.DumpStat();
	CA.DumpBlocks();
	CA.Free(memory);
	CA.Destroy();

	sizeless::SizelessAllocator SA;
	memory = (double*)SA.Alloc(sizeof(double*));
	SA.DumbStat();
	SA.DumbBlocks();	
	SA.Free(memory);

	return 0;
}