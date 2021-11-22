#pragma once

#include <Windows.h>
#include <assert.h>
#include "Page.h"
#include "Block.h"

class FixedSizedAllocator {
public:
	const int kHeaderOffset = sizeof(Page);

	FixedSizedAllocator(size_t block_size, int blocks_per_page);
	virtual ~FixedSizedAllocator();

	size_t GetSize();
	virtual void Init();
	virtual void Destroy();
	virtual void* Alloc();
	virtual void Free(void* p);

	virtual void DumbStat() const {
		// TODO:
	}

	virtual void DumbBlocks() const {
		// TODO:
	}

private:
	int blocks_per_page_;
	void* buffer_;
	size_t block_size_;
	size_t page_size_;

	Page* AllocPage();
	void FreePage(Page* page);
	Page* GetAvaliablePage();
	bool IsAvaliable(Page* page);
	void* GetPageDataAddress(Page* page);
	Block* ReserveAvaliableBlock(Page* page);
	Block* GetPageBlock(Page* page, int block_number);
	Page* GetBlockOwnerPage(const Block* block);
	bool IsBlockBelongToPage(Page* page, const void* block);
	bool IsFree(Page* page);
	void FreeBlock(Page* page, Block* block);
};