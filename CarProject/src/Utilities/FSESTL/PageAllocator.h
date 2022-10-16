//
//	PageAllocator.h
//	Balea Engine
//
//	Created by Diego Revilla on 19/05/21
//	Copyright © 2021 . All Rights reserved
//

#ifndef _PAGE_ALLOCATOR__H_
#define _PAGE_ALLOCATOR__H_

#include "CommonDefines.h"
#include "object_allocator.h"

namespace stl {
	template<typename TYPE>
	class PageAllocator {
	public:

		template<typename> friend class PageAllocator;

		using value_type = TYPE;
		typedef TYPE value_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef TYPE* pointer;
		typedef const TYPE* const_pointer;
		typedef TYPE& reference;
		typedef const TYPE& const_reference;
		template <class U1> struct rebind { typedef PageAllocator<U1> other; };
		PageAllocator(void) {
			static object_allocator
				galloc(sizeof(TYPE), object_allocator::oa_config());
			mOA = &galloc;
		};
		PageAllocator(const PageAllocator& other) : mOA(other.mOA) {};
		template <class U1> PageAllocator(const PageAllocator<U1>& other) : mOA(sizeof(TYPE), other.mOA.config()) {};
		constexpr TYPE inline* allocate(size_t size = 1) noexcept;

		template<typename ...Args>
		STATIC inline construct(TYPE* data, Args... ag) const noexcept;
		STATIC deallocate(TYPE* data, size_t = 1) noexcept;
		STATIC inline destroy(TYPE* data) noexcept;
	private:
		object_allocator* mOA;
	};

	// ------------------------------------------------------------------------
	/*! Allocate
	*
	*   Allocates sizeof(TYPE) * size bytes
	*/ //--------------------------------------------------------------------
	template<typename TYPE>
	constexpr TYPE inline* PageAllocator<TYPE>::allocate(size_t) noexcept {
#if 1
		return reinterpret_cast<TYPE*>(mOA->allocate());
#else
		return reinterpret_cast<TYPE*>(malloc(sizeof(TYPE)));
#endif
	}

	// ------------------------------------------------------------------------
	/*! Allocate
	*
	*   construct a variable of type TYPE
	*/ //-------------------------------------------------------------------
	template<typename TYPE>
	template<typename ...Args>
	inline STATIC PageAllocator<TYPE>::construct(TYPE* data, Args ...ag) const noexcept {
		new (data) TYPE(ag...);
	}

	// ------------------------------------------------------------------------
	/*! Deallocate
	*
	*   Deallocates the passed chunk
	*/ //-------------------------------------------------------------------
	template<typename TYPE>
	STATIC PageAllocator<TYPE>::deallocate(TYPE* data, size_t) noexcept {
#if 1
		static size_t dealloccount = 0;

		if (data) mOA->deallocate(data);
		dealloccount++;

		if (dealloccount >= 64) {
			mOA->free_empty_pages();
			dealloccount = 0;
		}
#else
		free(data);
#endif
	}

	// ------------------------------------------------------------------------
	/*! Destroy
	*
	*   Calls the TYPE destructor
	*/ //-------------------------------------------------------------------
	template<typename TYPE>
	STATIC inline PageAllocator<TYPE>::destroy(TYPE* data) noexcept {
		data->~TYPE();
	}
}

#endif
