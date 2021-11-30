#pragma once

#include <Windows.h>
#include <assert.h>
#include <map>

namespace sizeless {

struct Page {
	Page* next;
};

constexpr size_t kHeaderOffset = sizeof(Page);

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

#ifndef DEBUG
		allocated_[page] = size;
#endif

		page->next = page_;
		page_ = page;
		return GetDataAddress(page);
	}

	virtual void Free(void* p) {
		Page* page = GetPageAddress(p);
		assert(TryRemovePage(page) && "Not correct pointer.");

#ifndef DEBUG
		allocated_.erase(page);
#endif

		FreePage(page);
	}

	virtual bool IsBelongTo(void* p) {
		Page* page = GetPageAddress(p);

		for (Page* temp = page_; temp != nullptr; temp = temp->next) {
			if (page == temp) {
				return true;
			}
		}

		return false;
	}

#ifndef DEBUG
	virtual void DumbStat() const {
		std::cout << "SizelessAllocator statistics:\n";

		size_t count = 0;

		for (Page* page = page_; page != nullptr; page = page->next) {
			count += 1;
		}

		std::cout << "\tPages count:" << count << "\n";
		std::cout << "\n";
	}
#endif

#ifndef DEBUG
	virtual void DumbBlocks() const {
		std::cout << "SizelessAllocator pages:\n";

		size_t page_index = 0;

		for (Page* page = page_; page != nullptr; page = page->next) {
			const auto it = allocated_.find(page);

			std::cout << "\n\tPage " << page_index << " info:\n";
			std::cout << "\t\tAddress: " << page << " size:" << it->second << "\n";
		}

		std::cout << "\n";
	}
#endif

private:
#ifndef DEBUG
	std::map < Page*, size_t> allocated_;
#endif

	Page* page_;

	static Page* AllocPage(size_t size) {
		return (Page*)VirtualAlloc(
			NULL,
			kHeaderOffset + size,
			MEM_COMMIT | MEM_RESERVE,
			PAGE_READWRITE);
	}

	static void FreePage(Page* page) {
		VirtualFree(page, 0, MEM_RELEASE);
	}

	static void* GetDataAddress(const Page* page) {
		return (char*)page + kHeaderOffset;
	}

	static Page* GetPageAddress(const void* p) {
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
