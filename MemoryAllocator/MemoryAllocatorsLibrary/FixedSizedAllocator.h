#pragma once

#include <Windows.h>
#include <assert.h>
#include <iostream>
#include <vector>
#include <numeric>

namespace fixed {

struct Page {
	Page* next;
	int head;
	size_t initialized_count;
};

struct Block {
	int next;
};

const size_t kHeaderOffset = sizeof(Page);

template<size_t block_size>
class FixedSizedAllocator {
	static_assert(block_size >= sizeof(Block), "Block size should be greater then 4 bites.");

public:
	FixedSizedAllocator(size_t blocks_per_page)
		: page_(nullptr)
		, page_size_(kHeaderOffset + block_size * blocks_per_page)
		, blocks_per_page_(blocks_per_page)
	{}

	~FixedSizedAllocator() {
		assert(page_ == nullptr && "Allocator not deinitialized.");
	}

	size_t GetBlockSize() {
		return block_size;
	}

	void Init() {
		page_ = AllocPage();
	}

	void Destroy() {
		assert(page_ != nullptr && "Allocator not initialized.");

		Page* page = page_;

		while (page != nullptr) {
			assert(IsFree(page) && "Memory leak occurs.");
			Page* temp = page;
			page = page->next;
			FreePage(temp);
		}

		page_ = nullptr;
	}

	void* Alloc() {
		assert(page_ != nullptr && "Allocator not initialized.");

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
		assert(page_ != nullptr && "Allocator not initialized.");
		assert(p != nullptr && "Try free null.");

		if (p == nullptr) {
			return;
		}

		Block* block = (Block*)p;
		Page* page = GetBlockOwnerPage(block);

		assert(page != nullptr && "Not correct pointer.");

		if (page != nullptr) {
			FreeBlock(page, block);
		}
	}

	bool IsBelongTo(void* p) {
		Block* block = (Block*)p;
		return GetBlockOwnerPage(block) != nullptr;
	}

//#ifdef DEBUG
	virtual void DumbStat() const {
		std::cout << "FixedSizedAllocator<" << block_size << "> statistics:\n";
		size_t page_index = 0;
		size_t total_free = 0;
		size_t total_busy = 0;

		for (Page* page = page_; page != nullptr; page = page->next) {
			size_t free = CountFreeBlocks(page) + blocks_per_page_ - page->initialized_count;
			size_t busy = blocks_per_page_ - free;

			std::cout << "Page " << page_index << " statistics:\n";
			std::cout << "\tFree blocks: " << free << "\n";
			std::cout << "\tBusy blocks: " << busy << "\n";

			page_index += 1;
			total_free += free;
			total_busy += busy;
		}

		std::cout << "Total statistics:\n";
		std::cout << "\tPages count: " << page_index << "\n";
		std::cout << "\tFree blocks: " << total_free << "\n";
		std::cout << "\nBusy blocks: " << total_busy << "\n";
	}
//#endif

//#ifdef DEBUG
	virtual void DumbBlocks() const {
		std::cout << "FixedSizedAllocator<" << block_size << "> blocks:\n";
		size_t page_index = 0;

		for (Page* page = page_; page != nullptr; page = page->next) {
			std::vector<size_t> busy(page->initialized_count);
			std::iota(busy.begin(), busy.end(), 0);			
			Block* block;

			for (int i = page->head; i != -1; i = block->next) {
				block = GetPageBlock(page, i);
				busy.erase(std::next(busy.begin(), i));
			}

			for (size_t i : busy) {
				block = GetPageBlock(page, i);
				std::cout << "Page " << page_index << "Block " << i << ":\n";
				std::cout << "\tAddress: " << block << "Size: " << block_size << "\n";
			}

			page_index += 1;
		}
	}
//#endif

private:
	std::vector<Block*> allocated_;

	Page* page_;
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
		Page* page = page_;

		while (page->next != nullptr) {
			if (IsAvaliable(page)) {
				return page;
			}
			
			page = page->next;
		}

		if (IsAvaliable(page)) {
			return page;
		}

		page->next = AllocPage();
		return page->next;
	}

	bool IsAvaliable(Page* page) {
		return page->initialized_count < blocks_per_page_ || page->head != -1;
	}

	void* GetPageDataAddress(const Page* page) const {
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

	Block* GetPageBlock(const Page* page, int block_number) const {
		assert(block_number >= 0
			&& block_number < blocks_per_page_
			&& "Block number is out of page range.");

		void* page_data_address = GetPageDataAddress(page);
		void* block_address = (char*)page_data_address + block_size * block_number;
		return (Block*)block_address;
	}

	Page* GetBlockOwnerPage(const Block* block) {
		for (Page* page = page_; page != nullptr; page = page->next) {
			if (IsBlockBelongToPage(page, block)) {
				return page;
			}
		}

		return nullptr;
	}

	bool IsBlockBelongToPage(Page* page, const void* block) {
		return (char*)page <= (char*)block && (char*)block < (char*)page + page_size_;
	}

	bool IsFree(Page* page) {
		if (page->initialized_count == 0) {
			return true;
		}

		return CountFreeBlocks(page) == page->initialized_count;
	}

	size_t CountFreeBlocks(const Page* page) const {
		Block* block = GetPageBlock(page, page->head);
		size_t free_blocks_count = 1;

		while (block->next != -1) {
			block = GetPageBlock(page, block->next);
			free_blocks_count++;
		}

		return free_blocks_count;
	}

	void FreeBlock(Page* page, Block* block) {
		void* page_data_address = GetPageDataAddress(page);
		block->next = page->head;
		page->head = ((char*)block - (char*)page_data_address) / block_size;
	}
};

} // namespace fixed
