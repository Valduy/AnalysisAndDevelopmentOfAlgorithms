#pragma once

#include <Windows.h>
#include <assert.h>

namespace coalesce {

struct Block {
	size_t size;
	Block* prev;
	Block* next;	
};

struct Page {
	Page* next;
	Block* first;
	Block* last;
};

const size_t kHeaderOffset = sizeof(Page);
const size_t kBlockOffset = sizeof(size_t);
const size_t kMinDataSize = sizeof(Block) - kBlockOffset;

template<size_t buffer_size>
class CoalesceAllocator {
	static_assert(buffer_size >= sizeof(Block), "Buffer size is too small.");

public: 
	CoalesceAllocator() 
		: page_(nullptr)
		, page_size_(kHeaderOffset + buffer_size)
		, max_data_size_(buffer_size - kBlockOffset)
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

		Page* page = page_;

		while (page != nullptr) {
			assert(IsFree(page) && "Memory leak occurs.");
			Page* temp = page;
			page = (Page*)page->next;
			FreePage(temp);
		}

		page_ = nullptr;
	}

	virtual void* Alloc(size_t size) {
		assert(page_ != nullptr && "Allocator not initialized.");
		assert(size <= buffer_size && "Unable to alloc, size is too big.");

		if (buffer_size == 0) {
			return nullptr;
		}

		if (size > max_data_size_) {
			return nullptr;
		}

		Block* block = ReserveBlock(max(kMinDataSize, size));
		return GetDataAddress(block);
	}

	virtual void Free(void* p) {
		assert(page_ != nullptr && "Allocator not initialized.");
		assert(p != nullptr && "Try free null.");

		if (p == nullptr) {
			return;
		}

		Block* block = GetBlockAddress(p);
		Page* page = GetBlockOwnerPage(block);

		assert(page != nullptr && "Not correct pointer.");

		if (page != nullptr) {
			FreeBlock(page, block);
		}
	}

	virtual void DumpStat() const {

	}

	virtual void DumpBlocks() const {

	}

private:
	Page* page_;
	size_t page_size_;
	size_t max_data_size_;

	void* GetDataAddress(Block* block) {
		return (char*)block + kBlockOffset;
	}

	Block* GetBlockAddress(void* p) {
		return  (Block*)((char*)p - kBlockOffset);
	}

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
		page->first = (Block*)((char*) page + kHeaderOffset);
		page->last = page->first;
		Block* block = page->first;
		block->prev = nullptr;
		block->next = nullptr;
		block->size = max_data_size_;
		return page;
	}

	void FreePage(Page* page) {
		VirtualFree(page, 0, MEM_RELEASE);
	}

	Block* ReserveBlock(size_t size) {
		Page* page = page_;
		Block* block = nullptr;

		for (;page->next != nullptr; page = page->next)
		{
			if (TryGetAvaliableBlock(page, size, &block)) {
				return ReserveBlock(page, block, size);
			}
		}

		if (TryGetAvaliableBlock(page, size, &block)) {
			return ReserveBlock(page, block, size);
		}

		page->next = AllocPage();
		TryGetAvaliableBlock(page->next, size, &block);
		return ReserveBlock(page->next, block, size);
	}

	bool TryGetAvaliableBlock(Page* page, size_t size, Block** result) {
		*result = GetFirstAvaliableBlock(page, size);
		return *result != nullptr;
	}

	Block* GetFirstAvaliableBlock(Page* page, size_t size) {
		Block* block = page->first;

		for (Block* block = page->first; block != nullptr; block = block->next) {
			if (block->size >= size) {
				return block;
			}
		}

		return nullptr;
	}

	Block* ReserveBlock(Page* page, Block* block, size_t size) {
		// If: we can't cut piece of block.
		if (block->size - size <= kMinDataSize) {
			RemoveBlock(page, block);
			return block;
		}
		else {
			return CutPieceOfBlock(block, size);
		}
	}

	void RemoveBlock(Page* page, Block* block) {
		if (block->prev == nullptr) {
			page->first = block->next;
		}
		else {
			block->prev->next = block->next;
		}

		if (block->next == nullptr) {
			page->last = block->prev;
		}
		else {
			block->next->prev = block->prev;
		}
	}

	Block* CutPieceOfBlock(Block* block, size_t size) {
		Block* piece = (Block*)((char*)GetDataAddress(block) + block->size - size - kBlockOffset);
		block->size -= (size + kBlockOffset);
		piece->size = size;
		return piece;
	}

	Page* GetBlockOwnerPage(Block* block) {
		for (Page* page = page_; page != nullptr; page = page->next) {
			if (IsBlockBelongToPage(page, block)) {
				return page;
			}
		}

		return nullptr;
	}

	bool IsBlockBelongToPage(Page* page, const void* block) {
		return (char*)page <= (char*)block && (char*)block < (char*)(page + page_size_);
	}

	bool IsFree(Page* page) {
		Block* block = (Block*)((char*)page + kHeaderOffset);
		return block->size == max_data_size_;
	}

	void FreeBlock(Page* page, Block* block) {
		InsertBlock(page, block);
		TryMergeWithNeighbours(page, block);
	}

	void InsertBlock(Page* page, Block* block) {
		if (page->first == nullptr) {
			InsertBeginning(page, block);
			return;
		}
				
		for (Block* temp = page->first; temp != nullptr; temp = temp->next) {
			if (temp > block) {
				InsertBefore(page, temp, block);
				return;
			}
		}

		InsertEnd(page, block);
	}

	void InsertAfter(Page* page, Block* block, Block* new_block) {
		new_block->prev = block;
		new_block->next = block->next;

		if (block->next == nullptr) {
			page->last = new_block;
		}
		else {
			block->next->prev = new_block;
		}

		block->next = new_block;
	}

	void InsertBefore(Page* page, Block* block, Block* new_block) {
		new_block->next = block;
		new_block->prev = block->prev;

		if (block->prev == nullptr) {
			page->first = new_block;
		}
		else {
			block->prev->next = new_block;
		}

		block->prev = new_block;
	}

	void InsertBeginning(Page* page, Block* block) {
		if (page->first == nullptr) {
			page->first = block;
			page->last = block;
			block->prev = nullptr;
			block->next = nullptr;
		}
		else {
			InsertBefore(page, page->first, block);
		}
	}

	void InsertEnd(Page* page, Block* block) {
		if (page->last == nullptr) {
			InsertBeginning(page, block);
		}
		else {
			InsertAfter(page, page->last, block);
		}
	}

	void TryMergeWithNeighbours(Page* page, Block* block) {
		if (block->prev != nullptr && IsLeftNeighbour(block, block->prev)) {
			block = MergeWithNext(page, block->prev, block);
		}
		if (block->next != nullptr && IsRightNeighbour(block, block->next)) {
			block = MergeWithNext(page, block, block->next);
		}
	}

	bool IsLeftNeighbour(Block* block, Block* neighbour) {
		return (char*)neighbour + kBlockOffset + neighbour->size == (char*)block;
	}

	bool IsRightNeighbour(Block* block, Block* neighbour) {
		return (char*)block + kBlockOffset + block->size == (char*)neighbour;
	}

	Block* MergeWithNext(Page* page, Block* current, Block* next) {
		current->next = next->next;

		if (next->next != nullptr) {
			next->next->prev = current;
		}

		current->size += kBlockOffset + next->size;
		
		if (page->last == next) {
			page->last = current;
		}

		return current;
	}
};

} // namespace coalesce