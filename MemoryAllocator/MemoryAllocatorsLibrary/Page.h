#pragma once

struct Page {
	void* next;
	int head;
	int initialized_count;
};
