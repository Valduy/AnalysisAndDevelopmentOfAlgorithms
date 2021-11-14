#ifndef ARRAY_ARRAY_H_
#define ARRAY_ARRAY_H_

#include <cassert>
#include <utility>
#include <functional>
#include <algorithm>
#include <iostream>
#include <malloc.h>

template<typename T>
class Array final {
public:
	template<typename T>
	class ConstIter {
	public:
		using difference_type = T;
		using value_type = T;
		using pointer = const T*;
		using reference = const T&;
		using iterator_category = std::bidirectional_iterator_tag;

		ConstIter(T* first, T* last, std::function<T*(T*, int)> next, std::function<bool(T*, T*)> has_next)
			: temp_(first)
			, last_(last)
			, next_(next)
			, has_next_(has_next)
		{}

		const T& Get() const {
			return *temp_;
		}

		void Next() {
			temp_ = next_(temp_, 1);
		}

		void Prev() {
			temp_ = next_(temp_, -1);
		}

		bool HasCurrent() {
			return has_next_(temp_, last_);
		}

		ConstIter<T>& operator++() {
			Next();
			return *this;
		}

		ConstIter<T> operator++(int) {
			ConstIter<T> temp = *this;
			++*this;
			return temp;
		}

		ConstIter<T>& operator--() {
			Prev();
			return *this;
		}

		ConstIter<T> operator--(int) {
			ConstIter<T> temp = *this;
			--*this;
			return temp;
		}

		const T& operator*() const {
			return Get();
		}

		const T* operator->() const {
			return temp_;
		}

		friend bool operator < (const ConstIter<T>& lhs, const ConstIter<T>& rhs) {
			return lhs.temp_ < rhs.temp_;
		}

		friend bool operator > (const ConstIter<T>& lhs, const ConstIter<T>& rhs) {
			return rhs < lhs;
		}

		friend bool operator <= (const ConstIter<T>& lhs, const ConstIter<T>& rhs) {
			return !(lhs > rhs);
		}

		friend bool operator >= (const ConstIter<T>& lhs, const ConstIter<T>& rhs) {
			return !(lhs < rhs);
		}

		friend bool operator == (const ConstIter<T>& lhs, const ConstIter<T>& rhs) {
			return lhs.temp_ == rhs.temp_;
		}

		friend bool operator != (const ConstIter<T>& lhs, const ConstIter<T>& rhs) {
			return !(lhs == rhs);
		}

	protected:
		T* temp_;
		T* last_;
		std::function<T*(T*, int)> next_;
		std::function<bool(T*, T*)> has_next_;
	};

	template<typename T>
	class Iter : public ConstIter<T>{
	public:
		using difference_type = T;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::bidirectional_iterator_tag;

		Iter(T* first, T* last, std::function<T* (T*, int)> next, std::function<bool(T*, T*)> has_next)
			: ConstIterator(first, last, next, has_next)
		{}

		void Set(const T& value) {
			*(this->temp_) = value;
		}

		Iter<T>& operator++() {
			this->Next();
			return *this;
		}

		Iter<T> operator++(int) {
			Iter<T> temp = *this;
			++*this;
			return temp;
		}

		Iter<T>& operator--() {
			this->Prev();
			return *this;
		}

		Iter<T> operator--(int) {
			Iter<T> temp = *this;
			--*this;
			return temp;
		}

		T& operator*() {
			return *(this->temp_);
		}

		T* operator->() {
			return this->temp_;
		}
	};

	using ConstIterator = ConstIter<T>;
	using Iterator = Iter<T>;

	Array() 
		: Array(8)
	{}

	Array(int capacity) 
		: capacity_(capacity)
		, size_(0)
		, array_(AllocArray(capacity_))
	{}

	Array(const Array& other) {
		capacity_ = other.capacity_;
		size_ = other.size_;
		array_ = AllocArray(size_);

		for (int i = 0; i < size_; ++i) {
			array_[i] = other[i];
		}
	}

	Array(Array&& other) noexcept
		: Array() 
	{
		swap(*this, other);
	}

	~Array() {
		FreeArray(array_, size_);
	}

	int Insert(const T& value) {
		return Insert(size_, value);
	}

	int Insert(int index, const T& value) {
		assert(index >= 0 && index <= size_ && "Index out of range.");

		if (size_ == capacity_) {
			Resize(std::max(2 * size_, 1));
		}

		new(array_ + size_) T(value);
		BubbleDown(size_, size_ - index);
		++size_;
		return index;
	}

	void Remove(int index) {
		assert(index >= 0 && index < size_ && "Index out of range.");		
		int next_index = index + 1;
		ShiftLeft(next_index, size_ - next_index);
		array_[size_ - 1].~T();
		--size_;
	}

	int Size() const {
		return size_;
	}

	ConstIterator GetIterator() const {
		return ConstIterator(array_, array_ + size_ - 1, GetNextRight, HasNextRight);
	}

	Iterator GetIterator() {
		return Iterator(array_, array_ + size_ - 1, GetNextRight, HasNextRight);
	}

	ConstIterator GetReverceIterator() const {
		return ConstIterator(array_ + size_ - 1, array_, GetNextLeft, HasNextLeft);
	}

	Iterator GetReverceIterator() {
		return Iterator(array_ + size_ - 1, array_, GetNextLeft, HasNextLeft);
	}

	Iterator begin() {
		return GetIterator();
	}

	Iterator end() {
		return Iterator(array_ + size_, array_ + size_ - 1, GetNextRight, HasNextRight);
	}

	ConstIter cbegin() const {
		return GetIterator();
	}

	ConstIter cend() const {
		return ConstIterator(array_ + size_, array_ + size_ - 1, GetNextRight, HasNextRight);
	}

	friend void swap(Array<T>& first, Array<T>& second) {
		std::swap(first.capacity_, second.capacity_);
		std::swap(first.size_, second.size_);
		std::swap(first.array_, second.array_);
	}

	Array<T>& operator=(Array<T> other) {
		swap(*this, other);
		return *this;
	}

	T& operator[](int index) {
		assert(index >= 0 && index < size_ && "Index out of range.");
		return array_[index];
	}

private:
	int size_;
	int capacity_;
	T* array_;

	static T* GetNextRight(T* temp, int step) {
		return temp + step;
	}

	static T* GetNextLeft(T* temp, int step) {
		return temp - step;
	}

	static bool HasNextRight(const T* temp, const T* end) {
		return temp <= end;
	}

	static bool HasNextLeft(const T* temp, const T* end) {
		return temp >= end;
	}

	static T* AllocArray(int size) {
		return (T*)malloc(sizeof(T) * size);
	}

	static void FreeArray(T* arr, int size) {
		for (int i = 0; i < size; ++i) {
			arr[i].~T();
		}

		free(arr);
	}

	void Resize(int new_size) {
		T* new_array = AllocArray(new_size);

		for (int i = 0; i < size_; ++i) {
			new(new_array + i) T(std::move(array_[i]));
		}

		FreeArray(array_, size_);
		size_ = new_size;
		array_ = new_array;
	}	

	void BubbleDown(int index, int count) {
		T temp = std::move(array_[index]);

		for (int step = 0; step < count; ++step) {
			array_[index - step] = std::move(array_[index - step - 1]);
		}

		array_[index - count] = std::move(temp);
	}

	void ShiftLeft(int index, int count) {
		for (int i = index; i < index + count; ++i) {
			array_[i - 1] = std::move(array_[i]);
		}
	}
};

#endif // ARRAY_ARRAY_H_
