//
//	map.h
//	Balea Engine
//
//	Created by Diego Revilla on 19/05/21
//	Copyright © 2021 Digipen. All Rights reserved
//

#ifndef _MAP__H_
#define _MAP__H_

#include <utility>
#include "CommonDefines.h"
#include "Allocator.h"

namespace stl {
	template<typename T, typename U>
	struct _TreeLeaf {
		enum RBCOLOR {
			e_Red,
			e_Black
		};

		_TreeLeaf* left, * right, * parent;
		RBCOLOR color;
		std::pair<T, U> data;
	};

	template <typename K, typename V, class Alloc = allocator<_TreeLeaf<K, V>>>
	class Map {
	public:
		using storage_t = std::pair<K, V>;
		using Leaf = _TreeLeaf<K, V>;

		struct iterator {
		public:
			// ------------------------------------------------------------------------
			/*! Conversion Constructor
			*
			*   Makes an iterator from a pointer within the array
			*/ //--------------------------------------------------------------------
			iterator(storage_t* ptr) noexcept : ptr_(ptr) {};

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
			DONTDISCARD constexpr inline storage_t& operator*() const noexcept { return *ptr_; }

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
			DONTDISCARD constexpr storage_t inline* operator->() const noexcept { return ptr_; }

		private:
			storage_t* ptr_;
		};

		class MapException {
		public:

			enum class RBTREE_EXCEPTION : unsigned char { e_Duplicate, e_No_Memory };

			// ------------------------------------------------------------------------
			/*! Custom Constructor
			*
			*   Constructs an Exception with an Error code and a message
			*/ //----------------------------------------------------------------------
			constexpr MapException(const RBTREE_EXCEPTION ErrCode, const char* Message) noexcept :
				error_code_(ErrCode), message_(Message) {
			};

			// ------------------------------------------------------------------------
			/*! Code
			*
			*   Returns the code of the Exception
			*/ //----------------------------------------------------------------------
			DONTDISCARD constexpr virtual RBTREE_EXCEPTION code() const noexcept {
				return error_code_;
			}

			// ------------------------------------------------------------------------
			/*! What
			*
			*   Returns the message of the exception
			*/ //----------------------------------------------------------------------
			DONTDISCARD constexpr virtual const char* what() const noexcept {
				return message_;
			}

		private:
			RBTREE_EXCEPTION error_code_;
			const char* message_;
		};

		constexpr Map() noexcept;
		~Map() noexcept;
		STATIC insert(const storage_t& item);
		STATIC remove(const K& item) noexcept;
		STATIC clear() noexcept;
		DONTDISCARD iterator find(const K& value) const noexcept;
		DONTDISCARD constexpr bool empty() const noexcept;
		DONTDISCARD size_t inline size() const noexcept;
		DONTDISCARD size_t inline height() const noexcept;
		DONTDISCARD constexpr Leaf inline* root() const noexcept;
		DONTDISCARD constexpr iterator inline end() const noexcept;
		V& operator[](const K& key);

	private:
		STATIC rearrange_rotation(Leaf*& node, Leaf* direction, Leaf* previous) noexcept;
		STATIC rotate_left(Leaf* node) noexcept;
		STATIC rotate_right(Leaf* node) noexcept;
		DONTDISCARD size_t tree_height(const Leaf* root) const noexcept;
		DONTDISCARD size_t tree_size(const Leaf* root) const noexcept;
		STATIC clear_subtree(Leaf* root) noexcept;
		DONTDISCARD Leaf* uncle(const Leaf* node) const noexcept;
		DONTDISCARD Leaf* sibling(const Leaf* node) const noexcept;
		STATIC balanceblackblack(Leaf* x) noexcept;
		STATIC balanceredred(Leaf* node) noexcept;
		DONTDISCARD Leaf* sucessor(Leaf* x) const noexcept;
		Leaf* mRoot;
		Alloc mAllocator;
	};

	// ------------------------------------------------------------------------
	/*! Default Constructor
	*
	*   Constructs a new map
	*/ //----------------------------------------------------------------------
	template <typename K, typename V, class Alloc>
	constexpr Map<K, V, Alloc>::Map() noexcept :
		mRoot(nullptr) {
	}

	// ------------------------------------------------------------------------
	/*! Destructor
	*
	*   Frees all nodes of the tree to prevent memory leaking
	*/ //----------------------------------------------------------------------
	template <typename K, typename V, class Alloc>
	Map<K, V, Alloc>::~Map() noexcept {
		clear_subtree(mRoot);
	}

	// ------------------------------------------------------------------------
	/*! Insert
	*
	*   The item to insert inside the tree
	*/ //----------------------------------------------------------------------
	template <typename K, typename V, class Alloc>
	STATIC Map<K, V, Alloc>::insert(const storage_t& item) {
		Leaf* node;

		//Try to allocate a new node
		try {
			node = mAllocator.allocate();
			mAllocator.construct(node);
		}
		catch (...) {
			throw MapException(MapException::RBTREE_EXCEPTION::e_No_Memory, "Failed to allocate node");
		}

		node->data = item;
		node->parent = node->right = node->left = nullptr;

		/*Search for the insertion place*/
		//If we have a root, traverse the tree until we find the correct place
		if (mRoot) {
			Leaf* _iterator = mRoot;
			const auto fcheckandassign_ = [node, &_iterator](Leaf*& next) {
				if (!next) {
					next = node;
					node->parent = _iterator;
					_iterator = nullptr;
				}
				else _iterator = next;
			};

			do {
				//Should we proceed left?
				if (item.first < _iterator->data.first) fcheckandassign_(_iterator->left);
				//Should we proceed right?
				else if (_iterator->data.first < item.first) fcheckandassign_(_iterator->right);
				else throw MapException(MapException::RBTREE_EXCEPTION::e_Duplicate, "Duplicate Data");
			} while (_iterator);

			node->color = Leaf::RBCOLOR::e_Red;
			balanceredred(node);
		}
		//Else, we will create the root
		else
			mRoot = node;
		mRoot->color = Leaf::RBCOLOR::e_Black;
	}

	// ------------------------------------------------------------------------
	/*! Remove
	*
	*   Removes an item by it's key from the tree
	*/ //----------------------------------------------------------------------
	template <typename K, typename V, class Alloc>
	STATIC Map<K, V, Alloc>::remove(const K& item) noexcept {
		Leaf* node = nullptr;
		//If we have a tree
		if (mRoot) {
			//If we have a node
			for (Leaf* subtree_ = mRoot; subtree_;) {
				const storage_t& data_ = subtree_->data;

				//If we have to look on the right
				if (data_ < item) subtree_ = subtree_->right;

				//If we have to look on the left
				else if (item < data_) subtree_ = subtree_->left;

				//Else we are on the node
				else {
					node = subtree_;
					break;
				}
			}
		}

		if (!node) return;

		//If we are on the root
		if (node == mRoot) {
			//If we have no children, we have no tree
			if (!node->left && !node->right) {
				mRoot = nullptr;
				delete node;
				node = nullptr;
			}
			//If we have ONE children, replace it with the predecessor
			else if (!node->left || !node->right) {
				mRoot = sucessor(node);
				delete node;
				node = nullptr;
			}
		}

		//If we have a node
		if (node) {
			Leaf* sucessor_ = sucessor(node), * parent_
				= node->parent, * sibling_ = sibling(node);

			//Traverse from the node up to the root while we have two children,
			//swappin the value with it's sucessor
			for (; node && node->left && node->right;
				sucessor_ = sucessor(node), parent_ = node->parent,
				sibling_ = sibling(node)) {
				//If we have a predecessor
				storage_t temp = std::move(sucessor_->data);
				sucessor_->data = std::move(node->data);
				node->data = std::move(temp);
				node = sucessor_;
			}

			//If we have a sucessor, reaplace the current node with it
			if (sucessor_) {
				(parent_->left == node ? parent_->left : parent_->right) = sucessor_;
				sucessor_->parent = parent_;
				sucessor_->color = Leaf::RBColor::e_Black;
			}
			else {
				//If we have a black node, check for no double black collisions
				if (node->color == Leaf::RBColor::e_Black) balanceblackblack(node);

				//Else, change the siblind to RED
				else if (sibling_) sibling_->color = Leaf::RBColor::e_Red;
				(parent_->left == node ? parent_->left : parent_->right) = nullptr;
			}

			mAllocator.destroy(node);
			mAllocator.deallocate(node);
		}

		//The root should always be black
		if (mRoot)
			mRoot->color = Leaf::RBColor::e_Black;
	}

	// ------------------------------------------------------------------------
	/*! Balance Black Black
	*
	*   Balances the tree so that there is no two black nodes together
	*/ //----------------------------------------------------------------------
	template <typename K, typename V, class Alloc>
	STATIC Map<K, V, Alloc>::balanceblackblack(Leaf* node) noexcept {
		//While we are not on the root and the node is black (there is no need
		//to check if here since we did it in the calling function)
		do {
			Leaf* sibling_ = sibling(node), * parent = node->parent;

			//Traverse until we find a node with a sibling
			if (!sibling_) {
				node = parent;
				continue;
			}
			//If the sibling is black
			else if (sibling_->color != Leaf::RBColor::e_Black) {
				sibling_->color = Leaf::RBColor::e_Black;
				parent->color = Leaf::RBColor::e_Red;

				//If the sibling is on the left, rotate right
				if (sibling_ == sibling_->parent->left) rotate_right(parent);

				//Else rotate left
				else rotate_left(parent);

				continue;
			}

			//If the sibling is the left node
			if (sibling_ == sibling_->parent->left) {
				//If the sibling is Black
				if (sibling_->color != Leaf::RBColor::e_Black) {
					sibling_->color = Leaf::RBColor::e_Black;
					parent->color = Leaf::RBColor::e_Red;
					rotate_right(parent);
				}
				//Else, if the left child of the sibling is RED
				else if (sibling_->left && sibling_->left->color != Leaf::RBColor::e_Black) {
					sibling_->left->color = Leaf::RBColor::e_Black;
					rotate_right(parent);
					break;
				}
			}

			//Otherwise, is the right child of the sibling is black, set the parent to black
			if (sibling_->right && sibling_->right->color != Leaf::RBColor::e_Black) {
				//Aditionally, Set the Right children of the sibling to black and the sibling
				//to red in order not to break the rules. Perform two a left rotation
				if (sibling_ == sibling_->parent->right) {
					sibling_->right->color = Leaf::RBColor::e_Black;
					sibling_->color = Leaf::RBColor::e_Red;
					rotate_left(parent);
				}

				parent->color = Leaf::RBColor::e_Black;
				break;
			}
			sibling_->color = Leaf::RBColor::e_Red;

			auto& pcolor = parent->color;

			//If we still have a black node, we need to push up a level and
			//Recheck again
			if (pcolor == Leaf::RBColor::e_Black) node = parent;

			//Else, set the parents node to black and we are done
			else {
				pcolor = Leaf::RBColor::e_Black;
				break;
			}
		} while (node != mRoot && node->color == Leaf::RBColor::e_Black);
	}

	// ------------------------------------------------------------------------
	/*! Balance Red Red
	*
	*   Balances the tree so that there is no two red nodes together
	*/ //----------------------------------------------------------------------
	template<typename K, typename V, class Alloc>
	STATIC Map<K, V, Alloc>::balanceredred(Leaf* node) noexcept {
		Leaf* uncle_ = uncle(node), * parent_ = node->parent;

		//Once we have the node, recolor the tree
		//Remember: We need to solve only if two red nodes are contiguous until
		//we get to the root
		while (parent_ && node->color == Leaf::RBCOLOR::e_Red &&
			parent_->color == Leaf::RBCOLOR::e_Red) {
			Leaf* const grandparent_ = parent_->parent;

			auto& ucolor = uncle_->color;

			//If the uncle is red (Second situation according to the notes)
			if (uncle_ && ucolor == Leaf::RBCOLOR::e_Red) {
				ucolor = Leaf::RBCOLOR::e_Black;
				parent_->color = Leaf::RBCOLOR::e_Black;
				node = grandparent_;
				parent_ = node->parent;
				uncle_ = uncle(node);
			}
			else { //Else, we are on the first situation
				if (grandparent_->right == uncle_) {
					if (node == parent_->right) { //Should we Zag?
						rotate_left(parent_);
						node->color = Leaf::RBCOLOR::e_Black;
					}
					else
						parent_->color = Leaf::RBCOLOR::e_Black;

					rotate_right(grandparent_);
				}
				else {
					if (node == parent_->left) { //Should we Zig?
						rotate_right(parent_);
						node->color = Leaf::RBCOLOR::e_Black;
					}
					else
						parent_->color = Leaf::RBCOLOR::e_Black;

					rotate_left(grandparent_);
				}
			}
			grandparent_->color = Leaf::RBCOLOR::e_Red;
		}
	}

	// ------------------------------------------------------------------------
	/*! Sucessor
	*
	*   Returns the succesor of a node
	*/ //----------------------------------------------------------------------
	template <typename K, typename V, class Alloc>
	typename Map<K, V, Alloc>::Leaf* Map<K, V, Alloc>::sucessor(Leaf* node) const noexcept {
		//If we have a left child
		if (node->left) {
			node = node->left;

			//Go all the left you can
			while (node->right) node = node->right;
			return node;
		}

		//Else return the right (not technically the sucessor)
		return node->right;
	}

	// ------------------------------------------------------------------------
	/*! Clear
	*
	*   Clears a tree removing every element
	*/ //----------------------------------------------------------------------
	template <typename K, typename V, class Alloc>
	STATIC Map<K, V, Alloc>::clear() noexcept {
		clear_subtree(mRoot);
		mRoot = nullptr;
	}

	// ------------------------------------------------------------------------
	/*! Find
	*
	*   Returns if it can find a value and the number of comparisons needed
	*/ //----------------------------------------------------------------------
	template <typename K, typename V, class Alloc>
	typename Map<K, V, Alloc>::iterator Map<K, V, Alloc>::find(const K& value) const noexcept {
		//Do we have a tree?
		if (mRoot) {
			//Traverse the subtree
			for (Leaf* subtree_ = mRoot; subtree_;) {
				const storage_t& data_ = subtree_->data;

				//Should we proceed checking right?
				if (data_.first < value) subtree_ = subtree_->right;

				//Or left?
				else if (value < data_.first) subtree_ = subtree_->left;
				else return iterator(&subtree_->data);
			}
		}
		return iterator(nullptr);
	}

	// ------------------------------------------------------------------------
	/*! Empty
	*
	*   Returns wether the tree is empty or not
	*/ //----------------------------------------------------------------------
	template <typename K, typename V, class Alloc>
	constexpr bool Map<K, V, Alloc>::empty() const noexcept {
		return mRoot;
	}

	// ------------------------------------------------------------------------
	/*! Size
	*
	*   Returns the size of the tree
	*/ //----------------------------------------------------------------------
	template <typename K, typename V, class Alloc>
	size_t inline Map<K, V, Alloc>::size() const noexcept {
		return tree_size(mRoot);
	}

	// ------------------------------------------------------------------------
	/*! Height
	*
	*   The item to insert inside the tree
	*/ //----------------------------------------------------------------------
	template <typename K, typename V, class Alloc>
	size_t inline Map<K, V, Alloc>::height() const noexcept {
		return tree_height(mRoot);
	}

	// ------------------------------------------------------------------------
	/*! Root
	*
	*   Inserts an item into the Tree
	*/ //----------------------------------------------------------------------
	template <typename K, typename V, class Alloc>
	constexpr typename Map<K, V, Alloc>::Leaf inline* Map<K, V, Alloc>::root() const noexcept {
		return mRoot;
	}

	// ------------------------------------------------------------------------
	/*! End
	*
	*   Returns a dummy pointer iterator outside the RedBlackTree
	*/ //----------------------------------------------------------------------
	template <typename K, typename V, class Alloc>
	constexpr typename Map<K, V, Alloc>::iterator inline Map<K, V, Alloc>::end() const noexcept {
		return iterator(nullptr);
	}

	// ------------------------------------------------------------------------
	/*! Rearrange Rotation
	*
	*   Rearranges the parent and the sibling to match the rotation
	*/ //----------------------------------------------------------------------
	template <typename K, typename V, class Alloc>
	STATIC Map<K, V, Alloc>::rearrange_rotation(Leaf*& node, Leaf* direction, Leaf* previous) noexcept {
		if (direction) direction->parent = node;

		Leaf* const parent_ = node->parent;

		previous->parent = parent_;

		//Change the uncle to the other rotation node
		if (parent_) (parent_->left == node ? parent_->left : parent_->right) = previous;
		else mRoot = previous;
		node->parent = previous;
	}

	// ------------------------------------------------------------------------
	/*! Rotate Left
	*
	*   Rotates three nodes to the left
	*/ //----------------------------------------------------------------------
	template <typename K, typename V, class Alloc>
	STATIC Map<K, V, Alloc>::rotate_left(Leaf* node) noexcept {
		Leaf* temp_ = node->right;

		node->right = temp_->left;
		rearrange_rotation(node, node->right, temp_);
		temp_->left = node;
	}

	// ------------------------------------------------------------------------
	/*! Rotate Right
	*
	*   Rotates three nodes to the right
	*/ //----------------------------------------------------------------------
	template <typename K, typename V, class Alloc>
	STATIC Map<K, V, Alloc>::rotate_right(Leaf* node) noexcept {
		Leaf* temp_ = node->left;

		node->left = temp_->right;
		rearrange_rotation(node, node->left, temp_);
		temp_->right = node;
	}

	// ------------------------------------------------------------------------
	/*! Tree Height
	*
	*   Recursive function to find the height of the tree
	*/ //----------------------------------------------------------------------
	template <typename K, typename V, class Alloc>
	size_t Map<K, V, Alloc>::tree_height(const Leaf* root) const noexcept {
		//If we do not have a valid subtree
		if (!root) return -1;

		const int left_ = tree_height(root->left), right_ = tree_height(root->right);

		return left_ > right_ ? left_ + 1 : right_ + 1;
	}

	// ------------------------------------------------------------------------
	/*! Tree Size
	*
	*   Recursive function to find the size of the tree
	*/ //----------------------------------------------------------------------
	template <typename K, typename V, class Alloc>
	size_t Map<K, V, Alloc>::tree_size(const Leaf* root) const noexcept {
		//If we do not have a valid subtree
		if (!root) return 0;
		return tree_size(root->left) + tree_size(root->right) + 1;
	}

	// ------------------------------------------------------------------------
	/*! Clear Subtree
	*
	*   Recursive function to clear the tree
	*/ //----------------------------------------------------------------------
	template <typename K, typename V, class Alloc>
	STATIC Map<K, V, Alloc>::clear_subtree(Leaf* root) noexcept {
		//If we have a valid subtree
		if (root) {
			clear_subtree(root->right);
			clear_subtree(root->left);
			mAllocator.destroy(root);
			mAllocator.deallocate(root);
		}
	}

	// ------------------------------------------------------------------------
	/*! Uncle
	*
	*   Finds the uncle of a node
	*/ //----------------------------------------------------------------------
	template <typename K, typename V, class Alloc>
	typename Map<K, V, Alloc>::Leaf* Map<K, V, Alloc>::uncle(const Leaf* node) const
		noexcept {
		//If we have both a parent and a grandparent
		if (node->parent && node->parent->parent) {
			//Check on which side are we and return the other
			if (node->parent == node->parent->parent->right)
				return  node->parent->parent->left;
			else return  node->parent->parent->right;
		}

		return nullptr;
	}

	// ------------------------------------------------------------------------
	/*! Sibling
	*
	*   Finds the sibling of a node
	*/ //----------------------------------------------------------------------
	template <typename K, typename V, class Alloc>
	typename Map<K, V, Alloc>::Leaf* Map<K, V, Alloc>::sibling(const Leaf* node) const
		noexcept {
		if (node->parent) {
			if (node == node->parent->right)
				return  node->parent->left;
			else return  node->parent->right;
		}

		return nullptr;
	}

	// ------------------------------------------------------------------------
	/*! Operator[]
	*
	*   Finds a node with the key and return it's value. If there isn't one,
	*   create one new node with said key
	*/ //----------------------------------------------------------------------
	template <typename K, typename V, class Alloc>
	V& Map<K, V, Alloc>::operator[](const K& key) {
		Leaf* _iterator = mRoot;
		V* returned = nullptr;
		Leaf* node = nullptr;

		const auto fallocate_ = [this, &node, &_iterator, &key, &returned]() {
			//Try to allocate a new node
			try {
				node = mAllocator.allocate();
				mAllocator.construct(node);
			}
			catch (...) {
				throw MapException(MapException::RBTREE_EXCEPTION::e_No_Memory, "Failed to allocate node");
			}

			node->data = std::make_pair(key, V());
			returned = &node->data.second;
			node->parent = node->right = node->left = nullptr;
			node->parent = _iterator;
		};

		/*Search for the insertion place*/
		//If we have a root, traverse the tree until we find the correct place
		if (mRoot) {
			do {
				//Should we proceed left?
				if (key < _iterator->data.first) {
					if (!_iterator->left) {
						fallocate_();
						_iterator->left = node;
						_iterator = nullptr;
					}
					else _iterator = _iterator->left;
				}
				//Should we proceed right?
				else if (_iterator->data.first < key) {
					if (!_iterator->right) {
						fallocate_();
						_iterator->right = node;
						_iterator = nullptr;
					}
					else _iterator = _iterator->right;
				}
				else return _iterator->data.second;
			} while (_iterator);

			node->color = Leaf::RBCOLOR::e_Red;
			balanceredred(node);
		}
		//Else, we will create the root
		else {
			fallocate_();
			mRoot = node;
		}
		mRoot->color = Leaf::RBCOLOR::e_Black;
		return *returned;
	}
}

#endif