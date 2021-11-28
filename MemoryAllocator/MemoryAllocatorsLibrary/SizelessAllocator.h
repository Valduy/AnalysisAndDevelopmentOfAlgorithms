#pragma once

#include <Windows.h>
#include <assert.h>

namespace sizeless {

struct Page {
	Page* next;
};

const size_t kHeaderOffset = sizeof(Page);

class SizelessAllocator {
public:
	SizelessAllocator() 
		: page_(nullptr)
	{}

	virtual ~SizelessAllocator() {
		assert(page_ == nullptr && "Memory leak occurs.");
	}

	virtual void* Alloc(size_t size) {
		Page* page = AllocPage(size);

		if (page == nullptr) {
			return nullptr;
		}

		return GetDataAddress(page);
	}

	virtual void Free(void* p) {
		Page* page = GetPageAddress(p);
		assert(TryRemovePage(page) && "Not correct pointer.");
		FreePage(page);
	}

	virtual void DumbStat() const {

	}

	virtual void DumbBlocks() const {

	}

private:
	Page* page_;

	Page* AllocPage(size_t size) {
		Page* page = (Page*)VirtualAlloc(
			NULL,
			kHeaderOffset + size,
			MEM_COMMIT | MEM_RESERVE,
			PAGE_READWRITE);

		// If allocation failed.
		if (page == nullptr) {
			return nullptr;
		}

		page->next = page_;
		page_ = page;
		return page;
	}

	void FreePage(Page* page) {
		VirtualFree(page, 0, MEM_RELEASE);
	}

	void* GetDataAddress(Page* page) {
		return (char*)page + kHeaderOffset;
	}

	Page* GetPageAddress(void* p) {
		return (Page*)((char*)p - kHeaderOffset);
	}

	bool TryRemovePage(Page* page) {
		if (page_ == page) {
			page_ = page->next;
			return true;
		}

		Page* current = page_;
		Page* next = page_->next;

		while (next != nullptr) {
			if (next == page) {
				current->next = next->next;
				return true;
			}

			current = current->next;
			next = next->next;
		}

		return false;
	}
};

} // namespace sizeless
