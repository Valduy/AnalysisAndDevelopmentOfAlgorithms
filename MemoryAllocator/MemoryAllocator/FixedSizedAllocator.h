#ifndef MEMORYALLOCATOR_FIXEDSIZEDALLOCATOR_H_
#define MEMORYALLOCATOR_FIXEDSIZEDALLOCATOR_H_

#include <Windows.h>
#include <assert.h>
#include "Page.h"
#include "Block.h"

class FixedSizedAllocator {
public:
	FixedSizedAllocator(size_t block_size)
		: page_size_(0)
		, blocks_per_page_(0)
		, buffer_(nullptr)
		, block_size_(block_size)
	{}

	virtual ~FixedSizedAllocator() {

	}

	size_t GetSize() {
		return block_size_;
	}

	virtual void Init() {
		SYSTEM_INFO system_info;
		GetSystemInfo(&system_info);
		page_size_ = system_info.dwPageSize;

		blocks_per_page_ = (page_size_ - kHeaderOffset) / block_size_;

		buffer_ = CreatePage();
	}

	virtual void Destroy() {

	}

	virtual void* Alloc(size_t size) {
		Page* page = GetAvaliablePage();
		
		if (page == nullptr) {
			return nullptr;
		}

		Block* block = ReserveAvaliableBlock(page);
		return block;
	}

	virtual void Free(void *p) {
		Block* block = (Block*)p;
		Page* page = GetBlockOwnerPage(block);

		if (page != nullptr) {
			FreeBlock(page, block);
		}
	}

private:
	const int kHeaderOffset = sizeof(Page);
	
	int page_size_;
	int blocks_per_page_;
	void* buffer_;
	size_t block_size_;

	Page* CreatePage() {
		buffer_ = VirtualAlloc(
			NULL,
			page_size_,
			MEM_COMMIT | MEM_RESERVE,
			PAGE_READWRITE);

		// If allocation failed.
		if (buffer_ == nullptr) {
			return nullptr;
		}

		Page* page = (Page*)buffer_;
		page->next = nullptr;
		page->head = -1;
		page->initialized_count = 0;
	}

	Page* GetAvaliablePage() {
		Page* page = (Page*)buffer_;

		while (page->next != nullptr && IsAvaliable(page)) {
			page = (Page*)page->next;
		}

		if (IsAvaliable(page)) {
			return page;
		}

		page->next = CreatePage();
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
		else if (page->head != -1){
			Block* head = GetPageBlock(page, page->head);
			page->head = head->next;
			return head;			
		}
		
		return nullptr;
	}

	Block* GetPageBlock(Page* page, int block_number) {
		assert(block_number < 0
			&& kHeaderOffset + block_size_ * block_number < page_size_
			&& "Block number is out of page range.");

		void* page_data_address = GetPageDataAddress(page);
		void* block_address = (char*)page_data_address + block_size_ * block_number;
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
		return page <= block && block < page + page_size_;
	}

	void FreeBlock(Page* page, Block* block) {
		void* page_data_address = GetPageDataAddress(page);
		block->next = page->head;
		page->head = (block - page_data_address) / block_size_;
	}
};

#endif // MEMORYALLOCATOR_FIXEDSIZEDALLOCATOR_H_
