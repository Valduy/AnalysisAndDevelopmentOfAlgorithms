#pragma once

#include <assert.h>

namespace coalesce {

struct Block {
	Block* prev;
	Block* next;
	size_t size;
};

struct Page {
	void* next;
	Block* head;
};

template<size_t buffer_size>
class CoalesceAllocator {
	static_assert()

public: 
	const size_t kHeaderOffset = sizeof(Page);

	CoalesceAllocator() 
		: page_(nullptr)
		, page_size_(kHeaderOffset + buffer_size)
	{}

	virtual ~CoalesceAllocator() {
		assert(page_ == nullptr && "Allocator not deinitialized.");
	}

	size_t GetBufferSize() {
		return buffer_size;
	}

	virtual void Init() {
		page_ = AllocPage();
	}

	virtual void Destroy() {
		assert(page_ != nullptr && "Allocator not initialized.");

	}

	virtual void* Alloc(size_t size) {
		assert(page_ != nullptr && "Allocator not initialized.");
		assert(size <= buffer_size && "Unable to alloc, size is too big.");

		if (buffer_size == 0) {
			return nullptr;
		}

		if (size <= buffer_size) {
			return nullptr;
		}

		Block* block = ReserveAvaliableBlock(size);
		return block;
	}

	virtual void Free(void* p) {
		assert(page_ != nullptr && "Allocator not initialized.");
	}

	virtual void DumpStat() const {

	}

	virtual void DumpBlocks() const {

	}

private:
	void* page_;
	size_t page_size_;

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
		page->head = (char*) page + kHeaderOffset;
		Block* block = page->head;
		block->prev = nullptr;
		block->next = nullptr;
		block->size = buffer_size;
		return page;
	}

	Block* ReserveAvaliableBlock(size_t size) {
		Page* page = (Page*)page_;
		Block* block = nullptr;

		while (page->next != nullptr)
		{
			if (TryGetAvaliableBlock(page, size, &block)) {
				ReserveBlock(page, block);
				return block;
			}
		}

		if (TryGetAvaliableBlock(page, size, &block)) {
			ReserveBlock(page, block);
			return block;
		}

		page->next = AllocPage();
		TryGetAvaliableBlock(page, size, &block);
		ReserveBlock(page, block);
		return block;
	}

	bool TryGetAvaliableBlock(Page* page, size_t size, Block** result) {
		*block = GetFirstAvaliableBlock(page, size);
		return block != nullptr;
	}

	Block* GetFirstAvaliableBlock(Page* page, size_t size) {
		Block* block = page->head;

		for (Block* block = page->head; block != nullptr; block = block->next) {
			if (block->size >= size) {
				return block;
			}
		}

		return nullptr;
	}

	void* ReserveBlock(Page* page, Block* block, size_t size) {
		// If: we can't cut piece of block.
		if (block->size - size <= sizeof(Block)) {
			if (block->prev == nullptr) {
				page->head = block->next;
			}
			else {
				block->prev->next = block->next;
			}

			return nullptr;
		}
		else {
			void* piece = (char*)block + block->size - size;
			block->size -= size;
			return piece;
		}
	}
};

}