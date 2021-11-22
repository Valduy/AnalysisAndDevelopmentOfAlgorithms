#include "FixedSizedAllocator.h"

FixedSizedAllocator::FixedSizedAllocator(size_t block_size, int blocks_per_page)
	: page_size_(0)
	, blocks_per_page_(blocks_per_page)
	, buffer_(nullptr)
	, block_size_(block_size)
{}

FixedSizedAllocator::~FixedSizedAllocator() {
	assert(buffer_ == nullptr && "Allocator not deinitialized.");
}

size_t FixedSizedAllocator::GetSize() {
	return block_size_;
}

void FixedSizedAllocator::Init() {
	page_size_ = kHeaderOffset + block_size_ * blocks_per_page_;
	buffer_ = AllocPage();
}

void FixedSizedAllocator::Destroy() {
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

void* FixedSizedAllocator::Alloc() {
	assert(buffer_ != nullptr && "Allocator not initialized.");

	Page* page = GetAvaliablePage();

	if (page == nullptr) {
		return nullptr;
	}

	Block* block = ReserveAvaliableBlock(page);
	return block;
}

void FixedSizedAllocator::Free(void* p) {
	assert(buffer_ != nullptr && "Allocator not initialized.");

	Block* block = (Block*)p;
	Page* page = GetBlockOwnerPage(block);

	assert(page != nullptr && "Not correct pointer.");

	if (page != nullptr) {
		FreeBlock(page, block);
	}
}

Page* FixedSizedAllocator::AllocPage() {
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

void FixedSizedAllocator::FreePage(Page* page) {
	VirtualFree(page, 0, MEM_RELEASE);
}

Page* FixedSizedAllocator::GetAvaliablePage() {
	Page* page = (Page*)buffer_;

	while (page->next != nullptr && IsAvaliable(page)) {
		page = (Page*)page->next;
	}

	if (IsAvaliable(page)) {
		return page;
	}

	page->next = AllocPage();
	return (Page*)page->next;
}

bool FixedSizedAllocator::IsAvaliable(Page* page) {
	return page->initialized_count < blocks_per_page_ || page->head != -1;
}

void* FixedSizedAllocator::GetPageDataAddress(Page* page) {
	return (char*)page + kHeaderOffset;
}

Block* FixedSizedAllocator::ReserveAvaliableBlock(Page* page) {
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

Block* FixedSizedAllocator::GetPageBlock(Page* page, int block_number) {
	assert(block_number >= 0
		&& kHeaderOffset + block_size_ * block_number < page_size_
		&& "Block number is out of page range.");

	void* page_data_address = GetPageDataAddress(page);
	void* block_address = (char*)page_data_address + block_size_ * block_number;
	return (Block*)block_address;
}

Page* FixedSizedAllocator::GetBlockOwnerPage(const Block* block) {
	Page* page = (Page*)buffer_;

	while (page != nullptr && !IsBlockBelongToPage(page, block)) {
		page = (Page*)page->next;
	}

	return page;
}

bool FixedSizedAllocator::IsBlockBelongToPage(Page* page, const void* block) {
	return page <= block && block < page + page_size_;
}

bool FixedSizedAllocator::IsFree(Page* page) {
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

void FixedSizedAllocator::FreeBlock(Page* page, Block* block) {
	void* page_data_address = GetPageDataAddress(page);
	block->next = page->head;
	page->head = ((char*)block - (char*)page_data_address) / block_size_;
}