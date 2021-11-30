#pragma once

#include <Windows.h>
#include <assert.h>
#include <map>
#include <set>

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

constexpr size_t kHeaderOffset = sizeof(Page);
constexpr size_t kBlockOffset = sizeof(size_t);
constexpr size_t kMinDataSize = sizeof(Block) - kBlockOffset;

template<size_t buffer_size>
class CoalesceAllocator {
	static_assert(buffer_size >= sizeof(Block), "Buffer size is too small.");

public: 
	CoalesceAllocator() 
		: page_(nullptr)
	{}

	virtual ~CoalesceAllocator() {
		assert(page_ == nullptr && "Allocator not deinitialized.");
	}

	virtual void Init() {
		page_ = AllocPage(kPageSize, kMaxDataSize);
	}

	virtual void Destroy() {
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

	virtual void* Alloc(size_t size) {
		assert(page_ != nullptr && "Allocator not initialized.");
		assert(size <= buffer_size && "Unable to alloc, size is too big.");

		if (buffer_size == 0) {
			return nullptr;
		}

		if (size > kMaxDataSize) {
			return nullptr;
		}

		Block* block = ReserveBlock(max(kMinDataSize, size));
		
		if (block == nullptr) {
			return nullptr;
		}
		
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

#ifndef DEBUG
		allocated_[page].erase(block);
#endif

		if (page != nullptr) {
			FreeBlock(page, block);
		}
	}

	virtual bool IsBelongTo(void* p) {
		Block* block = GetBlockAddress(p);
		return GetBlockOwnerPage(block) != nullptr;
	}

#ifndef DEBUG
	virtual void DumpStat() const {
		std::cout << "CoalesceAllocator<" << buffer_size << "> statistics:\n";
		std::cout << "\tPage size:" << kPageSize << "\n";

		size_t page_index = 0;
		size_t total_free = 0;
		size_t total_busy = 0;

		for (Page* page = page_; page != nullptr; page = page->next) {			
			const size_t free = CountFreeBlocks(page);			
			const auto it = allocated_.find(page);
			const size_t busy = it != allocated_.end() ? it->second.size() : 0;

			std::cout << "\n\tPage " << page_index << " statistics:\n";
			std::cout << "\t\tFree blocks: " << free << "\n";
			std::cout << "\t\tBusy blocks: " << busy << "\n";

			page_index += 1;
			total_free += free;
			total_busy += busy;
		}

		std::cout << "\n\tTotal statistics:\n";
		std::cout << "\t\tPages count: " << page_index << "\n";
		std::cout << "\t\tFree blocks: " << total_free << "\n";
		std::cout << "\t\tBusy blocks: " << total_busy << "\n";
		std::cout << "\n";
	}
#endif

#ifndef DEBUG
	virtual void DumpBlocks() const {
		std::cout << "CoalesceAllocator<" << buffer_size << "> blocks:\n";

		size_t page_index = 0;

		for (Page* page = page_; page != nullptr; page = page->next) {
			std::cout << "\n\tPage " << page_index << " blocks:\n";

			const auto it = allocated_.find(page);

			if (it != allocated_.end()) {
				int i = 0;

				for (const auto block : it->second) {
					std::cout << "\t\tBlock: " << i << " Address: " << block << " Size: " << block->size << "\n";
					i += 1;
				}
			}

			page_index += 1;
		}

		std::cout << "\n";
	}
#endif

private:
#ifndef DEBUG
	std::map<Page*, std::set<Block*>> allocated_;
#endif

	const size_t kPageSize = kHeaderOffset + buffer_size;
	const size_t kMaxDataSize = buffer_size - kBlockOffset;

	Page* page_;

	static Page* AllocPage(size_t size, size_t max_data_size) {
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
		page->first = (Block*)((char*)page + kHeaderOffset);
		page->last = page->first;
		Block* block = page->first;
		block->prev = nullptr;
		block->next = nullptr;
		block->size = max_data_size;
		return page;
	}

	static void FreePage(Page* page) {
		VirtualFree(page, 0, MEM_RELEASE);
	}

	static void* GetDataAddress(const Block* block) {
		return (char*)block + kBlockOffset;
	}

	static Block* GetBlockAddress(const void* p) {
		return  (Block*)((char*)p - kBlockOffset);
	}

	static Block* GetFirstAvailableBlock(const Page* page, size_t size) {
		Block* block = page->first;

		for (Block* block = page->first; block != nullptr; block = block->next) {
			if (block->size >= size) {
				return block;
			}
		}

		return nullptr;
	}

	static bool TryGetAvailableBlock(const Page* page, size_t size, Block** result) {
		*result = GetFirstAvailableBlock(page, size);
		return *result != nullptr;
	}

	static void RemoveBlock(Page* page, const Block* block) {
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

	static Block* CutPieceOfBlock(Block* block, size_t size) {
		Block* piece = (Block*)((char*)GetDataAddress(block) + block->size - size - kBlockOffset);
		block->size -= (size + kBlockOffset);
		piece->size = size;
		return piece;
	}

	static void InsertBlock(Page* page, Block* block) {
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

	static void InsertAfter(Page* page, Block* block, Block* new_block) {
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

	static void InsertBefore(Page* page, Block* block, Block* new_block) {
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

	static void InsertBeginning(Page* page, Block* block) {
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

	static void InsertEnd(Page* page, Block* block) {
		if (page->last == nullptr) {
			InsertBeginning(page, block);
		}
		else {
			InsertAfter(page, page->last, block);
		}
	}

	Block* ReserveBlock(Page* page, Block* block, size_t size) {
		// If: we can't cut piece of block.
		if (block->size - size <= kMinDataSize) {
			RemoveBlock(page, block);

#ifndef DEBUG
			allocated_[page].insert(block);
#endif

			return block;
		}

		Block* piece = CutPieceOfBlock(block, size);

#ifndef DEBUG
		allocated_[page].insert(piece);
#endif

		return piece;
	}

	static void FreeBlock(Page* page, Block* block) {
		InsertBlock(page, block);
		TryMergeWithNeighbours(page, block);
	}

	static void TryMergeWithNeighbours(Page* page, Block* block) {
		if (block->prev != nullptr && IsLeftNeighbour(block, block->prev)) {
			block = MergeWithNext(page, block->prev, block);
		}
		if (block->next != nullptr && IsRightNeighbour(block, block->next)) {
			block = MergeWithNext(page, block, block->next);
		}
	}

	static bool IsLeftNeighbour(const Block* block, const Block* neighbour) {
		return (char*)neighbour + kBlockOffset + neighbour->size == (char*)block;
	}

	static bool IsRightNeighbour(const Block* block, const Block* neighbour) {
		return (char*)block + kBlockOffset + block->size == (char*)neighbour;
	}

	static Block* MergeWithNext(Page* page, Block* current, Block* next) {
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

	static size_t CountFreeBlocks(const Page* page)	{
		int free_blocks_count = 0;

		for (Block* block = page->first; block != nullptr; block = block->next) {
			free_blocks_count += 1;
		}

		return free_blocks_count;
	}

	Block* ReserveBlock(size_t size) {
		Page* page = page_;
		Block* block = nullptr;

		for (;page->next != nullptr; page = page->next) {
			if (TryGetAvailableBlock(page, size, &block)) {
				return ReserveBlock(page, block, size);
			}
		}

		if (TryGetAvailableBlock(page, size, &block)) {
			return ReserveBlock(page, block, size);
		}

		page->next = AllocPage(kPageSize, kMaxDataSize);

		if (page->next == nullptr) {
			return nullptr;
		}

		TryGetAvailableBlock(page->next, size, &block);
		return ReserveBlock(page->next, block, size);
	}
		
	Page* GetBlockOwnerPage(const Block* block) const {
		for (Page* page = page_; page != nullptr; page = page->next) {
			if (IsBlockBelongToPage(page, block)) {
				return page;
			}
		}

		return nullptr;
	}

	bool IsBlockBelongToPage(const Page* page, const void* block) const {
		return (char*)page <= (char*)block && (char*)block < (char*)page + kPageSize;
	}

	bool IsFree(Page* page) const {
		Block* block = (Block*)((char*)page + kHeaderOffset);
		return block->size == kMaxDataSize;
	}
};

} // namespace coalesce