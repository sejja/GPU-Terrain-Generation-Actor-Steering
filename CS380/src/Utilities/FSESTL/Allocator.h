//
//	Array.h
//	FSESTL
//
//	Created by Diego Revilla on 07/01/21
//	Copyright © 2021 Digipen. All Rights reserved
//

#ifndef _ALLOCATOR__H_
#define _ALLOCATOR__H_

namespace stl {
	template<typename TYPE>
	struct allocator {
		DONTDISCARD constexpr TYPE inline* allocate(const size_t size = 1) const noexcept;
		DONTDISCARD constexpr TYPE inline* reallocate(TYPE* mem, const size_t size = 1) const noexcept;
		
		template<typename ...Args>
		STATIC inline construct(TYPE* data, Args ...ag) const noexcept;
		STATIC inline deallocate(TYPE* data) const noexcept;
		STATIC inline destroy(const TYPE* data) const noexcept;
	};

	// ------------------------------------------------------------------------
	/*! Allocate
	*
	*   Allocates sizeof(TYPE) * size bytes
	*/ //--------------------------------------------------------------------
	template<typename TYPE>
	constexpr TYPE inline* allocator<TYPE>::allocate(const size_t size) const noexcept {
		return reinterpret_cast<TYPE*>(malloc(size * sizeof(TYPE)));
	}

	// ------------------------------------------------------------------------
	/*! Reallocate
	*
	*   Reallocates an ammount of memory extending it's size
	*/ //--------------------------------------------------------------------
	template<typename TYPE>
	constexpr TYPE inline* allocator<TYPE>::reallocate(TYPE* mem, const size_t size) const noexcept {
		return reinterpret_cast<TYPE*>(realloc(mem, size * sizeof(TYPE)));
	}

	// ------------------------------------------------------------------------
	/*! Allocate
	*
	*   construct a variable of type TYPE
	*/ //-------------------------------------------------------------------
	template<typename TYPE>
	template<typename ...Args>
	STATIC inline allocator<TYPE>::construct(TYPE* data, Args ...ag) const noexcept {
		new(data) TYPE(ag...);
	}

	// ------------------------------------------------------------------------
	/*! Deallocate
	*
	*   Deallocates the passed chunk
	*/ //-------------------------------------------------------------------
	template<typename TYPE>
	STATIC inline allocator<TYPE>::deallocate(TYPE* data) const noexcept {
		free(data);
	}

	// ------------------------------------------------------------------------
	/*! Destroy
	*
	*   Calls the TYPE destructor
	*/ //-------------------------------------------------------------------
	template<typename TYPE>
	STATIC inline allocator<TYPE>::destroy(const TYPE* data) const noexcept {
		data->~TYPE();
	}
}

#endif
