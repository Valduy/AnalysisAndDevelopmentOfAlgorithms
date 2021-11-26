#pragma once

#include <Windows.h>
#include <assert.h>
#include "Page.h"
#include "Block.h"

template<size_t block_size>
class FixedSizedAllocator {
	static_assert(block_size >= sizeof(Block), "Block size should be greater then 4 bites.");

public:
	const int kHeaderOffset = sizeof(Page);

	FixedSizedAllocator(size_t blocks_per_page)
		: page_size_(kHeaderOffset + block_size * blocks_per_page)
		, blocks_per_page_(blocks_per_page)
		, buffer_(nullptr)
	{}

	~FixedSizedAllocator() {
		assert(buffer_ == nullptr && "Allocator not deinitialized.");
	}

	size_t GetSize() {
		return block_size;
	}

	void Init() {
		buffer_ = AllocPage();
	}

	void Destroy() {
		assert(buffer_ != nullptr && "Allocator not initialized.");

		Page* page = (Page*)buffer_;

		while (page != nullptr) {
			assert(IsFree(page) && "Memory leak occurs.");
			Page* temp = page;
			page = (Page*)page->next;
			FreePage(temp);
		}

		buffer_ = nullptr;
	}

	void* Alloc() {
		assert(buffer_ != nullptr && "Allocator not initialized.");

		if (blocks_per_page_ == 0) {
			return nullptr;
		}

		Page* page = GetAvaliablePage();

		if (page == nullptr) {
			return nullptr;
		}

		Block* block = ReserveAvaliableBlock(page);
		return block;
	}

	void Free(void* p) {
		assert(buffer_ != nullptr && "Allocator not initialized.");

		Block* block = (Block*)p;
		Page* page = GetBlockOwnerPage(block);

		assert(page != nullptr && "Not correct pointer.");

		if (page != nullptr) {
			FreeBlock(page, block);
		}
	}

	virtual void DumbStat() const {
		// TODO:
	}

	virtual void DumbBlocks() const {
		// TODO:
	}

private:	
	void* buffer_;
	size_t page_size_;
	size_t blocks_per_page_;

	Page* AllocPage() {
		Page* page = (Page*)VirtualAlloc(
			NULL,
			page_size_,
			MEM_COMMIT | MEM_RESERVE,
			PAGE_READWRITE);

		// If allocation failed.
		if (page == nullptr) {
			return nullptr;
		}

		page->next = nullptr;
		page->head = -1;
		page->initialized_count = 0;
		return page;
	}

	void FreePage(Page* page) {
		VirtualFree(page, 0, MEM_RELEASE);
	}

	Page* GetAvaliablePage() {
		Page* page = (Page*)buffer_;

		while (page->next != nullptr && !IsAvaliable(page)) {
			page = (Page*)page->next;
		}

		if (IsAvaliable(page)) {
			return page;
		}

		page->next = AllocPage();
		return (Page*)page->next;
	}

	bool IsAvaliable(Page* page) {
		return page->initialized_count < blocks_per_page_ || page->head != -1;
	}

	void* GetPageDataAddress(Page* page) {
		return (char*)page + kHeaderOffset;
	}

	Block* ReserveAvaliableBlock(Page* page) {
		if (page->initialized_count < blocks_per_page_) {
			int current_uninitialized_block = page->initialized_count;
			page->initialized_count++;
			return GetPageBlock(page, current_uninitialized_block);
		}
		else if (page->head != -1) {
			Block* head = GetPageBlock(page, page->head);
			page->head = head->next;
			return head;
		}

		return nullptr;
	}

	Block* GetPageBlock(Page* page, int block_number) {
		assert(block_number >= 0
			&& block_number < blocks_per_page_
			&& "Block number is out of page range.");

		void* page_data_address = GetPageDataAddress(page);
		void* block_address = (char*)page_data_address + block_size * block_number;
		return (Block*)block_address;
	}

	Page* GetBlockOwnerPage(const Block* block) {
		Page* page = (Page*)buffer_;

		while (page != nullptr && !IsBlockBelongToPage(page, block)) {
			page = (Page*)page->next;
		}

		return page;
	}

	bool IsBlockBelongToPage(Page* page, const void* block) {
		return (char*)page <= (char*)block && (char*)block < (char*)(page + page_size_);
	}

	bool IsFree(Page* page) {
		if (page->initialized_count == 0) {
			return true;
		}

		Block* block = GetPageBlock(page, page->head);
		int blocks_count = 1;

		while (block->next != -1) {
			block = GetPageBlock(page, block->next);
			blocks_count++;
		}

		return blocks_count == page->initialized_count;
	}

	void FreeBlock(Page* page, Block* block) {
		void* page_data_address = GetPageDataAddress(page);
		block->next = page->head;
		page->head = ((char*)block - (char*)page_data_address) / block_size;
	}
};