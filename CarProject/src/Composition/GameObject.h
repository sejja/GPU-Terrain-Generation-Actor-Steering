//
//	GameObject.h
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 . All Rights reserved
//

#ifndef _GAME_OBJECT__H_
#define _GAME_OBJECT__H_

namespace BaleaEngine {
	struct Component;

	struct GameObject : public IBase, public Serializable {
	public:
#pragma region // Definitions
		friend class GCollector;

#pragma endregion

#pragma region // Constructors & Destructors
		GameObject();
		virtual ~GameObject();
#pragma endregion

#pragma region // Functions
		template<typename Ty_>
		DONTDISCARD Ty_* GetChild(const char* name) const noexcept;

		template<typename Ty_>
		DONTDISCARD Ty_* GetChild(const size_t idx) const noexcept;

		template<typename Ty_>
		DONTDISCARD Ty_* GetComponent(const char* name) const noexcept;

		template<typename Ty_>
		DONTDISCARD Ty_* GetComponentByType() const noexcept;

		template<typename Ty_>
		DONTDISCARD const Ty_* GetComponent(const size_t idx) const noexcept;

		template<typename... Args>
		DONTDISCARD GameObject* AddChild(const char* name = nullptr, Args... args);

		template<class K>
		DONTDISCARD bool HasComponentOfType() const noexcept;

		template<typename Ty_, typename... Args>
		DONTDISCARD Ty_* AddComponent(const char* name = nullptr, Args... args);

		void Destroy() override;
		DONTDISCARD size_t inline GetChildCount() const noexcept;
		DONTDISCARD size_t inline GetComponentCount() const noexcept;
		DONTDISCARD GameObject* GetParent() const noexcept;
		DONTDISCARD Math::Transform GetTransform() const noexcept;
		STATIC SetPosition(const Math::Vector3D& pos) noexcept;
		STATIC SetRotation(const Math::Vector3D& rot) noexcept;
		STATIC SetScale(const Math::Vector3D& scal) noexcept;
		DONTDISCARD Math::Vector3D inline GetPosition() const noexcept;
		DONTDISCARD Math::Vector3D inline GetRotation() const noexcept;
		DONTDISCARD Math::Vector3D inline GetScale() const noexcept;
		STATIC SetWorldPosition(const Math::Vector3D& pos) noexcept;
		STATIC SetWorldRotation(const Math::Vector3D& rot) noexcept;
		STATIC SetWorldScale(const Math::Vector3D& scal) noexcept;
		DONTDISCARD Math::Vector3D inline GetWorldPosition() const noexcept;
		DONTDISCARD Math::Vector3D inline GetWorldRotation() const noexcept;
		DONTDISCARD Math::Vector3D inline GetWorldScale() const noexcept;
		DONTDISCARD GameObject inline* GetChild(const size_t i) const noexcept;
		DONTDISCARD Component* GetComponent(const size_t i) const;
		STATIC inline AddTag(const char* flg) noexcept;
		DONTDISCARD glm::mat4 inline ModelMatrix() const noexcept;
		STATIC RemoveChild(GameObject* obj);
		STATIC AddComp(Component* obj) noexcept;
		STATIC RemoveComp(Component* obj);
		DONTDISCARD const bool HasTag(const char* flg) const noexcept;
		STATIC RemoveTag(const char* flg) noexcept;
		STATIC UpdateWorldTransform() noexcept;
		void ToJson(json& val) const noexcept;
		void FromJson(const json& val) noexcept;
#pragma endregion

#pragma region // Members
	private:
		Math::Transform mLocalTransform;
		Math::Transform mWorldTransform;
		stl::Array<GameObject*> mChilds;
		stl::Array<Component*> mComponents;
		stl::Array<const char*> mTags;
		GameObject* mParent = nullptr;
		glm::mat4 mModelMatrix;
		static stl::PageAllocator<GameObject> mGobjAlloc;
#pragma endregion
	};

	// ------------------------------------------------------------------------
	/*! Get World Position
	*
	*   Retrieves the world position
	*/ //----------------------------------------------------------------------
	DONTDISCARD Math::Vector3D inline GameObject::GetWorldPosition() const noexcept {
		return mWorldTransform.mPos;
	}

	// ------------------------------------------------------------------------
	/*! Get World Rotation
	*
	*   Retrieves the world rotation
	*/ //----------------------------------------------------------------------
	DONTDISCARD Math::Vector3D inline GameObject::GetWorldRotation() const noexcept {
		return mWorldTransform.mRot;
	}

	// ------------------------------------------------------------------------
	/*! Get World Scale
	*
	*   Retrieves the world scale
	*/ //----------------------------------------------------------------------
	DONTDISCARD Math::Vector3D inline GameObject::GetWorldScale() const noexcept {
		return mWorldTransform.mScale;
	}

	// ------------------------------------------------------------------------
	/*! Get Child
	*
	*   Gets the Child of a Game Object given the index
	*/ //--------------------------------------------------------------------
	GameObject inline* GameObject::GetChild(const size_t i) const noexcept {
		return mChilds[i];
	}

	// ------------------------------------------------------------------------
	/*! Get position
	*
	*   Returns the position of the Game Object
	*/ //--------------------------------------------------------------------
	Math::Vector3D inline GameObject::GetPosition() const noexcept {
		return mLocalTransform.mPos;
	}

	// ------------------------------------------------------------------------
	/*! Get Rotation
	*
	*   Returns the Rotation of a Game Object
	*/ //--------------------------------------------------------------------
	Math::Vector3D inline GameObject::GetRotation() const noexcept {
		return mLocalTransform.mRot;
	}

	// ------------------------------------------------------------------------
	/*! Get Scale
	*
	*   Gets the Scale of a Game Object
	*/ //--------------------------------------------------------------------
	Math::Vector3D inline GameObject::GetScale() const noexcept {
		return mLocalTransform.mScale;
	}

	// ------------------------------------------------------------------------
	/*! Get Child
	*
	*   Gets a Child contained within the Actor given it's name
	*/ //--------------------------------------------------------------------
	template<typename Ty_>
	Ty_* GameObject::GetChild(const char* name) const noexcept {
		//Seek through the childs
		for (auto& x : mChilds)

			//If we find an object with the same name
			if (x->GetName() == name)
				return dynamic_cast<Ty_*>(x);
		return nullptr;
	}

	// ------------------------------------------------------------------------
	/*! Get Child
	*
	*   Gets a child from the Actor given it's index
	*/ //--------------------------------------------------------------------
	template<typename Ty_>
	Ty_* GameObject::GetChild(const size_t idx) const noexcept {
		return (idx < mChilds.size() && idx >= 0) ?
			dynamic_cast<Ty_*>(mChilds[idx]) : nullptr;
	}

	// ------------------------------------------------------------------------
	/*! Get Component
	*
	*   Gets a Child contained within the Actor given it's name
	*/ //--------------------------------------------------------------------
	template<typename Ty_>
	Ty_* GameObject::GetComponent(const char* name) const noexcept {
		//Seek through the childs
		for (auto& x : mComponents)

			//If we find an object with the same name
			if (!strcmp(x->GetName(), name))
				return dynamic_cast<Ty_*>(x);
		return nullptr;
	}

	// ------------------------------------------------------------------------
	/*! Get Component By Type
	*
	*   Gets a Component by a given type
	*/ //--------------------------------------------------------------------
	template<typename Ty_>
	Ty_* GameObject::GetComponentByType() const noexcept {
		//For each component
		for (auto& x : mComponents)

			//If it's of type Ty_
			if (dynamic_cast<Ty_*>(x))
				return reinterpret_cast<Ty_*>(x);
		return nullptr;
	}

	// ------------------------------------------------------------------------
	/*! Get Child
	*
	*   Gets a child from the Actor given it's index
	*/ //--------------------------------------------------------------------
	template<typename Ty_>
	const Ty_* GameObject::GetComponent(const size_t idx) const noexcept {
		return (idx < mComponents.size() && idx >= 0) ?
			dynamic_cast<Ty_*>(mComponents[idx]) : nullptr;
	}

	// ------------------------------------------------------------------------
	/*! Get Child Count
	*
	*   Gets the count of how many children are there
	*/ //--------------------------------------------------------------------
	size_t inline GameObject::GetChildCount() const noexcept {
		return mChilds.size();
	}

	// ------------------------------------------------------------------------
	/*! Get Component Count
	*
	*   Gets the count of how many components are there
	*/ //--------------------------------------------------------------------
	size_t inline GameObject::GetComponentCount() const noexcept {
		return mComponents.size();
	}

	// ------------------------------------------------------------------------
	/*! Add Sub Object
	*
	*   Adds a SubObject to this actor
	*/ //--------------------------------------------------------------------
	template<typename... Args>
	GameObject* GameObject::AddChild(const char* name, Args... args) {
		GameObject* temp_ = mGobjAlloc.allocate();
		mGobjAlloc.construct(temp_, args...);

		temp_->SetName(name ? name : "No Name");
		temp_->mParent = this;
		temp_->Construct();

		if (Editor->isPlaying())
			temp_->Initialize();
		mChilds.emplace_back(temp_);

		return temp_;
	}

	// ------------------------------------------------------------------------
	/*! Has Component Of Type
	*
	*   Retusn wether a GameObject has a component of type or not
	*/ //--------------------------------------------------------------------
	template<class K>
	bool GameObject::HasComponentOfType() const noexcept {
		for (auto& x : mComponents)
			if (dynamic_cast<K*>(x))
				return true;
		return false;
	}

	// ------------------------------------------------------------------------
	/*! Add Sub Object
	*
	*   Adds a SubObject to this actor
	*/ //--------------------------------------------------------------------
	template<typename Ty_, typename... Args>
	Ty_* GameObject::AddComponent(const char* name, Args... args) {
		static stl::PageAllocator<Ty_> alloc;
		Ty_* temp_ = alloc.allocate();
		alloc.construct(temp_, args...);

		temp_->SetName(name ? name : "No Name");
		temp_->mOwner = this;
		//temp_->Construct();

		if (Editor->isPlaying())
			temp_->Initialize();
		mComponents.emplace_back(temp_);

		return temp_;
	}

	// ------------------------------------------------------------------------
	/*! Add Tag
	*
	*   Adds a Tag to the base class
	*/ //--------------------------------------------------------------------
	STATIC inline GameObject::AddTag(const char* flg) noexcept {
		mTags.push_back(flg);
	}

	// ------------------------------------------------------------------------
	/*! Model Matrix
	*
	*   Gets the Model Matrix of the GameObject
	*/ //--------------------------------------------------------------------
	glm::mat4 inline GameObject::ModelMatrix() const noexcept {
		return mModelMatrix;
	}
}

#endif