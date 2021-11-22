#ifndef MEMORYALLOCATOR_PAGE_H_
#define MEMORYALLOCATOR_PAGE_H_

struct Page {
	void* next;
	int head;
	int initialized_count;
};

#endif // MEMORYALLOCATOR_PAGE_H_