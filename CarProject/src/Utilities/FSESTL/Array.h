//
//	Array.h
//	FSESTL
//
//	Created by Diego Revilla on 07/01/21
//	Copyright © 2021 Digipen. All Rights reserved
//

#ifndef _ARRAY__H_
#define _ARRAY__H_

namespace stl {
#define SUBSCRIPT_ BadSubscript::

	class BadSubscript {
	public:

		using size_type = size_t;

		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*   Constructs a Subscript Error
		*/ //--------------------------------------------------------------------
		BadSubscript(size_type Subscript) noexcept : subscript_(Subscript) { }
		DONTDISCARD inline size_type GetSubscript() const noexcept;

	private:
		size_type subscript_;
	};

	// ------------------------------------------------------------------------
	/*! Get Subscript
	*
	*   Returns the Subscript
	*/ //--------------------------------------------------------------------
	inline SUBSCRIPT_ size_type BadSubscript::GetSubscript() const noexcept {
		return subscript_;
	}

#define ARRAY_  Array<TYPE, Alloc>::

	template<typename TYPE, class Alloc = allocator<TYPE>>
	class Array {
	public:

		using size_type = size_t;
		using value_type = TYPE;

		struct iterator {
		public:
			// ------------------------------------------------------------------------
			/*! Conversion Constructor
			*
			*   Makes an iterator from a pointer within the array
			*/ //--------------------------------------------------------------------
			iterator(TYPE* ptr) noexcept : ptr_(ptr) {};

			// ------------------------------------------------------------------------
			/*! Operator++
			*
			*   Increments the iterator to the next element
			*/ //--------------------------------------------------------------------
			constexpr inline iterator operator++() noexcept {
				++ptr_;
				return *this;
			}

			// ------------------------------------------------------------------------
			/*! Operator++
			*
			*   Increments the iterator to the next element (Wrapper)
			*/ //--------------------------------------------------------------------
			constexpr inline iterator operator++(int) noexcept { return operator++(); }

			// ------------------------------------------------------------------------
			/*! Operator!=
			*
			*   Checks two iterators to be not equal
			*/ //--------------------------------------------------------------------
			DONTDISCARD constexpr inline bool operator!=(const iterator& other) const noexcept {
				return ptr_ != other.ptr_;
			}

			// ------------------------------------------------------------------------
			/*! Operator==
			*
			*   Checks two iterators to be equal
			*/ //--------------------------------------------------------------------
			DONTDISCARD constexpr inline bool operator==(const iterator& other) const noexcept {
				return ptr_ == other.ptr_;
			}

			// ------------------------------------------------------------------------
			/*! Operator*
			*
			*   Dereferences an element within the Array
			*/ //--------------------------------------------------------------------
			DONTDISCARD constexpr inline TYPE& operator*() const noexcept { return *ptr_; }

			// ------------------------------------------------------------------------
			/*! Operator+
			*
			*   Fast forward "i" ammount of elements
			*/ //--------------------------------------------------------------------
			DONTDISCARD constexpr inline iterator operator+(size_t i) const noexcept {
				return iterator(ptr_ + i);
			}

			// ------------------------------------------------------------------------
			/*! Operator->
			*
			*   Get the contents of a iterator in memory location
			*/ //--------------------------------------------------------------------
			DONTDISCARD constexpr TYPE inline* operator->() const noexcept { return ptr_; }

		private:
			TYPE* ptr_;
		};

		inline Array() noexcept;
		~Array() noexcept;
		Array(const Array& rhs) noexcept;
		Array(const TYPE array[], const size_type size) noexcept;
		STATIC push_back(const TYPE& value) noexcept;
		STATIC push_front(const TYPE& value) noexcept;
		DONTDISCARD constexpr inline TYPE operator[](const size_type index) const noexcept;
		DONTDISCARD constexpr inline TYPE& operator[](const size_type index) noexcept;
		DONTDISCARD constexpr TYPE at(const size_type index) const;
		DONTDISCARD constexpr TYPE& at(const size_type index);
		DONTDISCARD constexpr iterator inline begin() const noexcept { return iterator(array_); }
		DONTDISCARD constexpr iterator inline end() const noexcept { return iterator(array_ + size_); }
		STATIC clear() noexcept;
		DONTDISCARD constexpr bool inline empty() const noexcept;
		DONTDISCARD constexpr size_type inline size() const noexcept;
		DONTDISCARD constexpr size_type inline capacity() const noexcept;
		STATIC inline pop_back() noexcept;
		STATIC pop_front() noexcept;
		STATIC insert(const TYPE& value, const unsigned position) noexcept;
		STATIC inline remove(const TYPE& value) noexcept;
		STATIC remove(const iterator value) noexcept;
		constexpr Array& operator=(const Array& rhs) noexcept;
		constexpr Array& operator+=(const Array& rhs) noexcept;
		DONTDISCARD constexpr Array operator+(const Array& rhs) const noexcept;
		STATIC reverse() noexcept;
		STATIC swap(Array& other) noexcept;
		DONTDISCARD constexpr bool operator==(const Array& rhs) const noexcept;
		STATIC shrink_to_fit() noexcept;
		STATIC sort() noexcept;
		STATIC reserve(const size_type cap) noexcept;
		STATIC resize(const size_type size) noexcept;
		DONTDISCARD constexpr TYPE inline& back() noexcept;
		DONTDISCARD constexpr inline TYPE back() const noexcept;
		DONTDISCARD constexpr TYPE inline* data() const noexcept;
		DONTDISCARD constexpr TYPE inline& front() noexcept;
		DONTDISCARD constexpr inline TYPE front() const noexcept;

		// ------------------------------------------------------------------------
		/*! Emplace Back
		*
		*   Constructs a type at the end of the array with the specified arguments
		*/ //----------------------------------------------------------------------
		template<typename... Args>
		STATIC emplace_back(Args&&... args) noexcept {
			capacity_grow_and_check();
			alcr_.construct(array_ + size_++, args...);
		}

	private:
		TYPE* array_;
		size_type size_;
		size_type capacity_;
#pragma warning (disable:4648)
		NOSPACE Alloc alcr_;
#pragma warning (default:4648)
		STATIC check_bounds(size_type index) const;
		STATIC SwapE(TYPE& a, TYPE& b) const noexcept;
		STATIC capacity_grow_and_check() noexcept;
		template<class T> struct remove_reference { using type = T; };
		template<class T> struct remove_reference<T&> { using type = T; };
		template<class T> struct remove_reference<T&&> { using type = T; };
	};

	// ------------------------------------------------------------------------
	/*! Default Constructor
	*
	*   Constructs an Array Class
	*/ //----------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	inline Array<TYPE, Alloc>::Array() noexcept :
		array_(nullptr), size_(0), capacity_(0) {
	}

	// ------------------------------------------------------------------------
	/*! Custom Constructor
	*
	*   Constructs an array with a stack array
	*/ //-------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	Array<TYPE, Alloc>::Array(const TYPE array[], const size_type size) noexcept :
		array_(alcr_.allocate(size)), capacity_(size) {
		//iterate through all the array adding the elements
		for (size_ = 0; size_ < size;)
			new (array_ + size_) TYPE(array[size_++]);
	}

	// ------------------------------------------------------------------------
	/*! Copy Constructor
	*
	*   Copies an Array into the another
	*/ //-------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	Array<TYPE, Alloc>::Array(const Array& rhs) noexcept {
		new (this) Array(rhs.array_, rhs.size_);
	}

	// ------------------------------------------------------------------------
	/*! Destructor
	*
	*   Destroys an Array Class
	*/ //-------------------------------------------------------------------
	template<typename T, class Alloc>
	Array<T, Alloc>::~Array() noexcept {
		clear();
	}

	// ------------------------------------------------------------------------
	/*! Push Back
	*
	*   Adds a new element to the end of the Array
	*/ //-------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	STATIC Array<TYPE, Alloc>::push_back(const TYPE& value) noexcept {
		emplace_back(static_cast<typename remove_reference<decltype(value)>::type&&>(value));
	}

	// ------------------------------------------------------------------------
	/*! Push Front
	*
	*   Adds a new element to the front of the Array
	*/ //-------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	STATIC Array<TYPE, Alloc>::push_front(const TYPE& value) noexcept {
		capacity_grow_and_check();
		memcpy(array_ + 1, array_, sizeof(TYPE) * size_);
		*array_ = value, size_++;
	}

	// ------------------------------------------------------------------------
	/*! Operator[]
	*
	*   Acess certain element of the array
	*/ //-------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	constexpr inline TYPE Array<TYPE, Alloc>::operator[](const size_type index) const noexcept {
		return array_[index];
	}

	// ------------------------------------------------------------------------
	/*! Operator[]
	*
	*   Acess certain element of the array
	*/ //-------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	constexpr inline TYPE& Array<TYPE, Alloc>::operator[](const size_type index) noexcept {
		return array_[index];
	}

	// ------------------------------------------------------------------------
	/*! At
	*
	*   Acess certain element of the array with bound cheking
	*/ //-------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	constexpr TYPE Array<TYPE, Alloc>::at(const size_type index) const {
		check_bounds(index);
		return array_[index];
	}

	// ------------------------------------------------------------------------
	/*! At
	*
	*   Acess certain element of the array with bound cheking
	*/ //-------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	constexpr TYPE& Array<TYPE, Alloc>::at(const size_type index) {
		check_bounds(index);
		return array_[index];
	}

	// ------------------------------------------------------------------------
	/*! Clear
	*
	*   Clears everything on the array
	*/ //----------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	STATIC Array<TYPE, Alloc>::clear() noexcept {
		//Destroy all elements
		for (size_type i = 0; i < size_; i++)
			alcr_.destroy(array_ + i);
		alcr_.deallocate(array_);
		capacity_ = size_ = 0, array_ = nullptr;
	}

	// ------------------------------------------------------------------------
	/*! Empty
	*
	*   Is the Array empty?
	*/ //----------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	constexpr bool inline Array<TYPE, Alloc>::empty() const noexcept {
		return !size_;
	}

	// ------------------------------------------------------------------------
	/*! Size
	*
	*   Returns the size of the Array
	*/ //----------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	constexpr size_t inline Array<TYPE, Alloc>::size() const noexcept {
		return size_;
	}

	// ------------------------------------------------------------------------
	/*! Capacity
	*
	*   Returns the array capacity
	*/ //----------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	constexpr size_t inline Array<TYPE, Alloc>::capacity() const noexcept {
		return capacity_;
	}

	// ------------------------------------------------------------------------
	/*! Swap
	*
	*   Swaps two vectors
	*/ //----------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	STATIC Array<TYPE, Alloc>::swap(Array& other) noexcept {
		SwapE(*this, other);
	}

	// ------------------------------------------------------------------------
	/*! Reverse
	*
	*   Reverses an Array
	*/ //----------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	STATIC Array<TYPE, Alloc>::reverse() noexcept {
		const size_type hsize_ = size_ / 2;
		//go through half the elements of this list
		for (size_type i = 0; i < hsize_; i++)
			SwapE((*this)[i], (*this)[size_ - i - 1]);
	}

	// ------------------------------------------------------------------------
	/*! Pop Back
	*
	*   Removes the last element of the Array
	*/ //----------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	STATIC inline Array<TYPE, Alloc>::pop_back() noexcept {
		alcr_.destroy(array_ + --size_);
	}

	// ------------------------------------------------------------------------
	/*! Operator=
	*
	*   Assigns one vector to another
	*/ //----------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	constexpr Array<TYPE, Alloc>& Array<TYPE, Alloc>::operator=(const Array& rhs) noexcept {
		//Destroy all elements
		for (size_type i = 0; i < size_; i++)
			alcr_.destroy(array_ + i);
		new (this) Array(rhs);

		return *this;
	}

	// ------------------------------------------------------------------------
	/*! Operator +=
	*
	*   Adds one vector to this one
	*/ //----------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	constexpr Array<TYPE, Alloc>& Array<TYPE, Alloc>::operator+=(const Array& rhs)
		noexcept {
		const size_type newsize_ = size_ + rhs.size_;
		const TYPE* _temp = array_ + size;
		reserve(newsize_);
		for (size_type i = 0; i < rhs.size_;)
			alcr_.construct(_temp + i, rhs.array_[i++]);
		size_ = newsize_;

		return *this;
	}

	// ------------------------------------------------------------------------
	/*! Operator +
	*
	*   Adds two vectors
	*/ //----------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	constexpr Array<TYPE, Alloc> Array<TYPE, Alloc>::operator+(const Array& rhs)
		const noexcept {
		Array temp_;

		temp_.size_ = size_ + rhs.size_;
		temp_.reserve(temp_.size_);
		memcpy(temp_.array_, array_, sizeof(TYPE) * size_);
		memcpy(temp_.array_ + size_, rhs.array_, sizeof(TYPE) * rhs.size_);

		return temp_;
	}

	// ------------------------------------------------------------------------
	/*! Remove
	*
	*   Removes the first occurrence of an array
	*/ //----------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	STATIC Array<TYPE, Alloc>::remove(const TYPE& i) noexcept {
		//for every single object
		for (size_type j = 0; j < size_; j++)
			//if we found a match
			if (operator[](j) == i) {
				TYPE* const offset_ = array_ + j;

				memcpy(offset_, offset_ + 1, sizeof(TYPE) * (size_-- - 1 - j));
				return;
			}
	}

	// ------------------------------------------------------------------------
	/*! Remove
	*
	*   Removes the first occurrence of an array
	*/ //----------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	STATIC Array<TYPE, Alloc>::remove(const iterator i) noexcept {
		TYPE* ptr = &(*i);
		memcpy(ptr, ptr + 1, sizeof(TYPE) * ((array_ + size_-- - 1) - ptr));
	}

	// ------------------------------------------------------------------------
	/*! Pop Front
	*
	*   Pops the first element of the list
	*/ //---------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	STATIC Array<TYPE, Alloc>::pop_front() noexcept {
		alcr_.destroy(array_);
		memcpy(array_, array_ + 1, sizeof(TYPE) * --size_);
	}

	// ------------------------------------------------------------------------
	/*! Insert
	*
	*   Insert an element into the array at a desired position
	*/ //---------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	STATIC Array<TYPE, Alloc>::insert(const TYPE& value, const unsigned position) noexcept {
		//if we are in an out of range index
		check_bounds(position);
		size_++;
		capacity_grow_and_check();
		TYPE* offset_ = array_ + position;
		memcpy(offset_ + 1, offset_, sizeof(TYPE) * (size_ - 1 - position));
		(*this)[position] = value;
	}

	// ------------------------------------------------------------------------
	/*! Shrink to Fit
	*
	*   Removes overhead allocation
	*/ //---------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	STATIC Array<TYPE, Alloc>::shrink_to_fit() noexcept {
		//If we do not match the capacity already
		if (size_ != capacity_)
			array_ = alcr_.reallocate(array_, size_ = capacity_);
	}

	// ------------------------------------------------------------------------
	/*! Sorts
	*
	*   Sorts the vector (requires < operator to be implemented)
	*/ //----------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	STATIC Array<TYPE, Alloc>::sort() noexcept {
		//go through all the values in the vector
		for (size_type i = 1; i < size_; i++) {
			size_type pos = i - 1;

			//for each iteration, get the minimun of the values ahead
			for (size_type j = i; j < size_; j++)
				//if we found a new smaller value, update the position
				if ((*this)[j] < (*this)[pos])
					pos = j;

			//if this value is smaller than the one we are about to swap
			if ((*this)[pos] < (*this)[i])
				SwapE((*this)[pos], (*this)[i]);
		}
	}

	// ------------------------------------------------------------------------
	/*! Reserve
	*
	*   Reserves memory space
	*/ //----------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	STATIC Array<TYPE, Alloc>::reserve(const size_type cap) noexcept {
		if (cap > capacity_)
			array_ = alcr_.reallocate(array_, capacity_ = cap);
	}

	// ------------------------------------------------------------------------
	/*! Resize
	*
	*   Resizes the Array to contain N elements
	*/ //----------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	STATIC Array<TYPE, Alloc>::resize(const size_type size) noexcept {
		//If we want to have more elements
		if (size > size_) {
			reserve(size);

			//Construct the remaining elements
			for (size_type i = size_; i < size; i++)
				emplace_back(static_cast<typename remove_reference<TYPE>::type&&>(TYPE()));

			//else, if we want fewer elements
		}
		else if (size < size_)
			//Trim the surplus
			for (size_type i = size_ - 1; i >= size; i--)
				operator[](i).~TYPE();
		size_ = size;
	}

	// ------------------------------------------------------------------------
	/*! Operator==
	*
	*   Reserves memory space
	*/ //----------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	constexpr bool Array<TYPE, Alloc>::operator==(const Array& rhs) const noexcept {
		//If the vectors have different sizes
		if (size_ != rhs.size_)
			return false;

		return !memcmp(array_, rhs.array_, sizeof(TYPE) * size_);
	}

	// ------------------------------------------------------------------------
	/*! Check Bounds
	*
	*   Throws an exception if we are trying to access out of bounds
	*/ //----------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	STATIC Array<TYPE, Alloc>::check_bounds(const size_type index) const {
		// Don't have to check for < 0 because index is unsigned
		if (index >= size_)
			throw BadSubscript(index);
	}

	// ------------------------------------------------------------------------
	/*! Back
	*
	*   Returns the last element of the array
	*/ //----------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	constexpr TYPE inline& Array<TYPE, Alloc>::back() noexcept {
		return *(array_ + size_ - 1);
	}

	// ------------------------------------------------------------------------
	/*! Back
	*
	*   Returns the last element of the array
	*/ //----------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	constexpr inline TYPE Array<TYPE, Alloc>::back() const noexcept {
		return *(array_ + size_ - 1);
	}

	// ------------------------------------------------------------------------
	/*! Data
	*
	*   Increments the iterator to the next element (Wrapper)
	*/ //--------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	constexpr TYPE inline* Array<TYPE, Alloc>::data() const noexcept {
		return array_;
	}

	// ------------------------------------------------------------------------
	/*! Front
	*
	*   Returns the first element of the array
	*/ //--------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	constexpr inline TYPE Array<TYPE, Alloc>::front() const noexcept {
		return array_[0];
	}

	// ------------------------------------------------------------------------
	/*! Front
	*
	*   Returns the first element of the array
	*/ //--------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	constexpr TYPE inline& Array<TYPE, Alloc>::front()  noexcept {
		return array_[0];
	}

	// ------------------------------------------------------------------------
	/*! Swap
	*
	*   Swaps two elements
	*/ //-------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	STATIC Array<TYPE, Alloc>::SwapE(TYPE& a, TYPE& b) const noexcept {
		TYPE temp = static_cast<typename remove_reference<TYPE>::type&&>(a);
		a = static_cast<typename remove_reference<TYPE>::type&&>(b);
		b = static_cast<typename remove_reference<TYPE>::type&&>(temp);
	}

	// ------------------------------------------------------------------------
	/*! Capacity_grow_and_check
	*
	*   Grows the vector if needed
	*/ //----------------------------------------------------------------------
	template<typename TYPE, class Alloc>
	STATIC Array<TYPE, Alloc>::capacity_grow_and_check() noexcept {
		//If we do not have more space
		if (size_ >= capacity_)
			array_ = alcr_.reallocate(array_, (capacity_ = capacity_ ? capacity_ * 2 : 1) * 2);
	}
}

#endif