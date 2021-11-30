#pragma once

#include "FixedSizedAllocator.h"
#include "CoalesceAllocator.h"
#include "SizelessAllocator.h"

namespace manager {

const size_t kMaxCoalesceSize = 4096 * 3 - coalesce::kBlockOffset;

class MemoryAllocator {
public:
	MemoryAllocator()
		: granularity_(GetGranularity())
		, FSA16_((granularity_ - fixed::kHeaderOffset) / 16)
		, FSA32_((granularity_ - fixed::kHeaderOffset) / 32)
		, FSA64_((granularity_ - fixed::kHeaderOffset) / 64)
		, FSA128_((granularity_ * 2 - fixed::kHeaderOffset) / 128)
		, FSA256_((granularity_ * 4 - fixed::kHeaderOffset) / 256)
		, FSA512_((granularity_ * 8 - fixed::kHeaderOffset) / 512)
	{}

	virtual ~MemoryAllocator() = default;

	virtual void Init() {
		FSA16_.Init();
		FSA32_.Init();
		FSA64_.Init();
		FSA128_.Init();
		FSA256_.Init();
		FSA512_.Init();
		CA_.Init();		
	}

	virtual void Destroy() {
		FSA16_.Destroy();
		FSA32_.Destroy();
		FSA64_.Destroy();
		FSA128_.Destroy();
		FSA256_.Destroy();
		FSA512_.Destroy();
		CA_.Destroy();
	}

	virtual void* Alloc(size_t size) {
		if (size <= 16) {
			return FSA16_.Alloc();
		}
		if (size <= 32) {
			return FSA32_.Alloc();
		}
		if (size <= 64) {
			return FSA64_.Alloc();
		}
		if (size <= 128) {
			return FSA128_.Alloc();
		}
		if (size <= 256) {
			return FSA256_.Alloc();
		}
		if (size <= 512) {
			return FSA512_.Alloc();
		}
		if (size <= kMaxCoalesceSize) {
			return CA_.Alloc(size);
		}

		return SA_.Alloc(size);
	}

	virtual void Free(void* p) {
		if (FSA16_.IsBelongTo(p)) {
			FSA16_.Free(p);
		}
		else if (FSA32_.IsBelongTo(p)) {
			FSA32_.Free(p);
		}
		else if (FSA64_.IsBelongTo(p)) {
			FSA64_.Free(p);
		}
		else if (FSA128_.IsBelongTo(p)) {
			FSA128_.Free(p);
		}
		else if (FSA256_.IsBelongTo(p)) {
			FSA256_.Free(p);
		}
		else if (FSA512_.IsBelongTo(p)) {
			FSA512_.Free(p);
		}
		else if (CA_.IsBelongTo(p)) {
			CA_.Free(p);
		}
		else if (SA_.IsBelongTo(p)) {
			SA_.Free(p);
		}
	}

#ifndef DEBUG
	virtual void DumpStat() const {
		FSA16_.DumbStat();
		FSA32_.DumbStat();
		FSA64_.DumbStat();
		FSA128_.DumbStat();
		FSA256_.DumbStat();
		FSA512_.DumbStat();
		CA_.DumpStat();
		SA_.DumbStat();
	}
#endif

#ifndef DEBUG
	virtual void DumpBlocks() const {
		FSA16_.DumbBlocks();
		FSA32_.DumbBlocks();
		FSA64_.DumbBlocks();
		FSA128_.DumbBlocks();
		FSA256_.DumbBlocks();
		FSA512_.DumbBlocks();
		CA_.DumpBlocks();
		SA_.DumbBlocks();
	}
#endif

private:
	size_t granularity_;

	fixed::FixedSizedAllocator<16> FSA16_;
	fixed::FixedSizedAllocator<32> FSA32_;
	fixed::FixedSizedAllocator<64> FSA64_;
	fixed::FixedSizedAllocator<128> FSA128_;
	fixed::FixedSizedAllocator<256> FSA256_;
	fixed::FixedSizedAllocator<512> FSA512_;

	coalesce::CoalesceAllocator<kMaxCoalesceSize> CA_;

	sizeless::SizelessAllocator SA_;

	static size_t GetGranularity() {
		SYSTEM_INFO info;
		GetSystemInfo(&info);
		return info.dwPageSize;
	}
};

} // namespace manager

