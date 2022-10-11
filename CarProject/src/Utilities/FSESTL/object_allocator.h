/**
* @file object_allocator.h
* @author Diego Revilla, 540000819 , diego.r@digipen.edu
* @date 2021/01/08
* @brief implementation for the object allocator
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#ifndef OBJECTALLOCATORH
#define OBJECTALLOCATORH

#include <string>
#include "CommonDefines.h"

// This memory manager class
class object_allocator {
public:

	struct oa_config {
		struct header_block_info {
			size_t      m_size;

			header_block_info() : m_size(1) {
			};
		};

		oa_config(unsigned ObjectsPerPage = 64,
			unsigned MaxPages = 0xFFFF,
			header_block_info const& HBInfo = header_block_info(),
			unsigned Alignment = 8) :
			m_objects_per_page(ObjectsPerPage),
			m_max_pages(MaxPages),
			m_hblock_info(HBInfo),
			m_alignment(Alignment)
		{
			m_left_align_size = 0;
			m_inter_align_size = 0;
		}

		unsigned m_objects_per_page;
		unsigned m_max_pages;
		header_block_info m_hblock_info;
		unsigned m_alignment;
		unsigned m_left_align_size;
		unsigned m_inter_align_size;
	};

	class oa_exception {
	public:
		// Possible exception codes
		enum OA_EXCEPTION
		{
			E_NO_MEMORY,      // out of physical memory (operator new fails)
			E_NO_PAGES,       // out of logical memory (max pages has been reached)
			E_BAD_BOUNDARY,   // block address is on a page, but not on any block-boundary
			E_MULTIPLE_FREE,  // block has already been freed
			E_CORRUPTED_BLOCK // block has been corrupted (pad bytes have been overwritten)
		};

		oa_exception(OA_EXCEPTION ErrCode, std::string const& Message) :
			m_error_code(ErrCode), m_message(Message) {};

		virtual ~oa_exception() = default;

		OA_EXCEPTION        code() const { return m_error_code; }
		virtual char const* what() const { return m_message.c_str(); }

	private:
		OA_EXCEPTION m_error_code;
		std::string  m_message;
	};

	// object_allocator statistical info
	struct oa_stats {
		oa_stats(void) :
			m_object_size(0), m_page_size(0), m_free_objects(0)
		{};

		size_t   m_object_size;    // size of each object
		size_t   m_page_size;      // size of a page including all headers, padding, etc.
		unsigned m_free_objects;   // number of objects on the free list
		unsigned m_pages_in_use;   // number of pages allocated
	};

	// This allows us to easily treat raw objects as nodes in a linked list
	struct generic_object
	{
		generic_object* next;
	};

	// Creates the ObjectManager per the specified values
	// Throws an exception if the construction fails. (Memory allocation problem)
	object_allocator(size_t object_size, oa_config const& config);

	// Prevent copy construction and assignment
	object_allocator(object_allocator const& oa) = delete;
	object_allocator& operator=(object_allocator const& oa) = delete;

	// Destroys the ObjectManager (never throws)
	~object_allocator();

	// Take an object from the free list and give it to the client (simulates new)
	// Throws an exception if the object can't be allocated. (Memory allocation problem)
	void* allocate(char const* label = 0);

	// Returns an object to the free list for the client (simulates delete)
	// Throws an exception if the the object can't be freed. (Invalid object)
	void deallocate(void* object);

	// Frees all empty pages (extra credit)
	unsigned free_empty_pages();

private:
	void allocate_new_page_();
	void configure_headers(generic_object* page) noexcept;
	bool is_object_allocated(generic_object* object) const noexcept;
	void inline header_increase_use_count(generic_object* object,
		const char* label) noexcept;
	void inline header_decrease_use_count(generic_object* object) noexcept;
	generic_object inline* get_object_ptr(generic_object* page,
		size_t object) const noexcept;
	generic_object* set_page_ptr_at_index(generic_object* page, size_t index,
		generic_object* ptr) noexcept;
	void inline* get_object_header(generic_object* object) const noexcept;
	bool is_page_unused(generic_object* page) const noexcept;
	void purge_page_from_freelist(generic_object* pageAddr) noexcept;

	oa_config mConfig;
	oa_stats mStat;
	generic_object* mHeadBlock;
	generic_object* mFreeList;
};

#endif
