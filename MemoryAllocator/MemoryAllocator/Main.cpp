#include <Windows.h>
#include "../MemoryAllocatorsLibrary/FixedSizedAllocator.h"

int main() {
	FixedSizedAllocator<sizeof(double)> FSA(5);
	FSA.Init();
	double* memory = (double*)FSA.Alloc();
	*memory = 9.1;
	FSA.Free(memory);
	FSA.Destroy();
	return 0;
}