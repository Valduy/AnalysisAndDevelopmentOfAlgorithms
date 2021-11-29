#pragma once

#include <Windows.h>
#include <assert.h>
#include <iostream>
#include <algorithm>

namespace fixed {

struct Page {
	Page* next;
	int head;
	size_t initialized_count;
};

struct Block {
	int next;
};

constexpr size_t kHeaderOffset = sizeof(Page);

template<size_t block_size>
class FixedSizedAllocator {
	static_assert(block_size >= sizeof(Block), "Block size should be greater then 4 bites.");

public:
	FixedSizedAllocator(size_t blocks_per_page)
		: page_(nullptr)
		, page_size_(kHeaderOffset + block_size * blocks_per_page)
		, blocks_per_page_(blocks_per_page)
	{}

	virtual ~FixedSizedAllocator() {
		assert(page_ == nullptr && "Allocator not deinitialized.");
	}

	void Init() {
		page_ = AllocPage(page_size_);
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

		Page* page = GetAvailablePage();

		if (page == nullptr) {
			return nullptr;
		}

		Block* block = ReserveAvailableBlock(page);
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

	bool IsBelongTo(void* p) const {
		const Block* block = (Block*)p;
		return GetBlockOwnerPage(block) != nullptr;
	}

	virtual void DumbStat() const {
		std::cout << "FixedSizedAllocator<" << block_size << "> statistics:\n";
		std::cout << "\tPage size: " << page_size_ << "\n";
		std::cout << "\tBlock size: " << block_size << "\n";

		size_t page_index = 0;
		size_t total_free = 0;
		size_t total_uninitialized = 0;
		size_t total_busy = 0;

		for (Page* page = page_; page != nullptr; page = page->next) {
			const size_t uninitialized = blocks_per_page_ - page->initialized_count;
			const size_t free = CountFreeBlocks(page);			
			const size_t busy = blocks_per_page_ - free;

			std::cout << "\n\tPage " << page_index << " statistics:\n";
			std::cout << "\t\tUninitialized blocks:" << uninitialized << "\n";
			std::cout << "\t\tFree blocks: " << free << "\n";			
			std::cout << "\t\tBusy blocks: " << busy << "\n";

			page_index += 1;
			total_uninitialized += uninitialized;
			total_free += free;			
			total_busy += busy;
		}

		std::cout << "\n\tTotal statistics:\n";
		std::cout << "\t\tPages count: " << page_index << "\n";
		std::cout << "\t\tUninitialized blocks:" << total_uninitialized << "\n";
		std::cout << "\t\tFree blocks: " << total_free << "\n";		
		std::cout << "\t\tBusy blocks: " << total_busy << "\n";
		std::cout << "\n";
	}

	virtual void DumbBlocks() const {
		std::cout << "FixedSizedAllocator<" << block_size << "> blocks:\n";
		bool* busy = new bool[blocks_per_page_];
		size_t page_index = 0;

		for (Page* page = page_; page != nullptr; page = page->next) {
			std::cout << "\n\tPage " << page_index << " statistics:\n";
			std::fill_n(busy, blocks_per_page_, true);
		
			Block* block;

			for (int i = page->head; i != -1; i = block->next) {
				busy[i] = false;
				block = GetPageBlock(page, i);
			}

			for (int i = 0; i < page->initialized_count; ++i) {
				if (busy[i]) {
					block = GetPageBlock(page, i);
					std::cout << "\t\tBlock: " << i << " Address: " << block << " Size: " << block_size << "\n";
				}
			}

			page_index += 1;
		}

		delete[] busy;
		std::cout << "\n";
	}

private:
	Page* page_;
	size_t page_size_;
	size_t blocks_per_page_;

	static Page* AllocPage(size_t size) {
		Page* page = (Page*)VirtualAlloc(
			NULL,
			size,
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

	static void FreePage(Page* page) {
		VirtualFree(page, 0, MEM_RELEASE);
	}

	Page* GetAvailablePage() const {
		Page* page = page_;

		while (page->next != nullptr) {
			if (IsAvailable(page)) {
				return page;
			}
			
			page = page->next;
		}

		if (IsAvailable(page)) {
			return page;
		}

		page->next = AllocPage(page_size_);
		return page->next;
	}

	bool IsAvailable(const Page* page) const {
		return page->initialized_count < blocks_per_page_ || page->head != -1;
	}

	void* GetPageDataAddress(const Page* page) const {
		return (char*)page + kHeaderOffset;
	}

	Block* ReserveAvailableBlock(Page* page) const {
		if (page->initialized_count < blocks_per_page_) {
			const int new_block = page->initialized_count;
			page->initialized_count++;
			return GetPageBlock(page, new_block);
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

	Page* GetBlockOwnerPage(const Block* block) const {
		for (Page* page = page_; page != nullptr; page = page->next) {
			if (IsBlockBelongToPage(page, block)) {
				return page;
			}
		}

		return nullptr;
	}

	bool IsBlockBelongToPage(Page* page, const void* block) const {
		return (char*)page <= (char*)block && (char*)block < (char*)page + page_size_;
	}

	bool IsFree( const Page* page) const {
		if (page->initialized_count == 0) {
			return true;
		}

		return CountFreeBlocks(page) == page->initialized_count;
	}

	size_t CountFreeBlocks(const Page* page) const {
		if (page->head == -1) {
			return 0;
		}

		const Block* block = GetPageBlock(page, page->head);
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
