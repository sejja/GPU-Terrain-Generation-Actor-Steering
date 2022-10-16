//
//	GameObject.cpp
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 . All Rights reserved
//

#include <Shared.h>

namespace BaleaEngine {
	stl::PageAllocator<GameObject> GameObject::mGobjAlloc;

	// ------------------------------------------------------------------------
	/*! Default Constructor
	*
	*   Adds the GameObject to the Object list
	*/ //--------------------------------------------------------------------
	GameObject::GameObject() :
		mModelMatrix(1.f) {
		SetScale({ 1, 1, 1 });
	}

	// ------------------------------------------------------------------------
	/*! Destructor
	*
	*   Removes the Children to ensure no memory leaks
	*/ //--------------------------------------------------------------------
	GameObject::~GameObject() {

		static stl::PageAllocator<GameObject> alloc; static stl::PageAllocator<Component> allocc;

		stl::FForEach(mChilds.begin(), mChilds.end(), [](GameObject* x) { 
			alloc.destroy(x); alloc.deallocate(x);
		});
		stl::FForEach(mComponents.begin(), mComponents.end(), [](Component* x) { 
			allocc.destroy(x); allocc.deallocate(x);
		});
	}

	// ------------------------------------------------------------------------
	/*! Remove Child
	*
	*   Removes a Child from the Actor
	*/ //--------------------------------------------------------------------
	STATIC GameObject::RemoveChild(GameObject* obj) {
		GCollector::Instance().AddChildToRemove(obj);
	}

	// ------------------------------------------------------------------------
	/*! Add Component
	*
	*   Adds an already created Component to the GameObject
	*/ //--------------------------------------------------------------------
	STATIC GameObject::AddComp(Component* obj) noexcept {
		obj->mOwner = this;
		mComponents.emplace_back(obj);
		obj->Initialize();

		//If we are currently playing
		if (Editor->isPlaying())
			ObjMgr->beginqueue.push_back(obj);
	}

	// ------------------------------------------------------------------------
	/*! Get Parent
	*
	*   Gets the Parent of the Actor
	*/ //--------------------------------------------------------------------
	GameObject* GameObject::GetParent() const noexcept {
		return mParent;
	}

	// ------------------------------------------------------------------------
	/*! Remove Component
	*
	*   Removes a Component from our Childs
	*/ //--------------------------------------------------------------------
	STATIC GameObject::RemoveComp(Component* obj) {
		GCollector::Instance().AddComponentToRemove(obj);
	}

#pragma region TRANSFORM MANAGEMENT
	// ------------------------------------------------------------------------
	/*! Set position
	*
	*   Sets the position of the Game Object
	*/
	DONTDISCARD Math::Transform GameObject::GetTransform() const noexcept
	{
		return mWorldTransform;
	}
	//--------------------------------------------------------------------
	STATIC GameObject::SetPosition(const Math::Vector3D& pos) noexcept {
		mLocalTransform.mPos = pos;
		UpdateWorldTransform();
	}

	// ------------------------------------------------------------------------
	/*! Set Rotation
	*
	*   Sets the Rotation of the Game Object
	*/ //--------------------------------------------------------------------
	STATIC GameObject::SetRotation(const Math::Vector3D& rot) noexcept {
		mLocalTransform.mRot = rot;
		UpdateWorldTransform();
	}

	// ------------------------------------------------------------------------
	/*! Set Scale
	*
	*   Sets the Scale of the Game Object
	*/ //--------------------------------------------------------------------
	STATIC GameObject::SetScale(const Math::Vector3D& scal) noexcept {
		mLocalTransform.mScale = scal;
		UpdateWorldTransform();
	}

	// ------------------------------------------------------------------------
	/*! Set World Position
	*
	*   Sets the Position in World Space
	*/ //--------------------------------------------------------------------
	STATIC GameObject::SetWorldPosition(const Math::Vector3D& pos) noexcept {
		// set the local position so that the world position is equal to pos;
		if (!mParent) mLocalTransform.mPos = pos;
		else mLocalTransform.mPos = mParent->mWorldTransform.InverseMultiplyPoint(pos);
		mWorldTransform.mPos = pos;
	}

	// ------------------------------------------------------------------------
	/*! Set World Rotation
	*
	*   Sets the Rotation in World Space
	*/ //--------------------------------------------------------------------
	STATIC GameObject::SetWorldRotation(const Math::Vector3D& rot) noexcept {
		if (!mParent) mLocalTransform.mRot = rot;
		else mLocalTransform.mRot = mParent->mWorldTransform.InverseConcatenation(
			{ Math::Vector3D{ 0,0,0 }, rot, Math::Vector3D{ 1,1,1 } }).mRot;
		mWorldTransform.mRot = rot;
	}

	// ------------------------------------------------------------------------
	/*! Set World Scale
	*
	*   Sets the Scale in world space
	*/ //--------------------------------------------------------------------
	STATIC GameObject::SetWorldScale(const Math::Vector3D& scale) noexcept {
		if (!mParent) mLocalTransform.mScale = scale;
		else mLocalTransform.mScale = mParent->mWorldTransform.InverseConcatenation({
			Math::Vector3D{ 0.f,0.f,0.f }, Math::Vector3D{ 0.0f, 0.f, 0.f }, Math::Vector3D(scale) }).mScale;
		mWorldTransform.mScale = scale;
	}

	// ------------------------------------------------------------------------
	/*! Get Component
	*
	*   Gets the Components of a Game Object given the index
	*/ //--------------------------------------------------------------------
	Component* GameObject::GetComponent(const size_t i) const {
		if (i < mComponents.size())
			return mComponents[i];
		else
			throw std::exception("No such component");
	}

	// ------------------------------------------------------------------------
	/*! Update World Transform
	*
	*   Updates the World Transform of a GameObject
	*/ //--------------------------------------------------------------------
	STATIC GameObject::UpdateWorldTransform() noexcept {
		if (mParent) {
			mWorldTransform.mPos = mParent->mWorldTransform.mPos + mLocalTransform.mPos;
			mWorldTransform.mRot = mParent->mWorldTransform.mRot + mLocalTransform.mRot;
			mWorldTransform.mScale = mParent->mWorldTransform.mScale * mLocalTransform.mScale;
		}
		else
			mWorldTransform = mLocalTransform;

		mModelMatrix = mWorldTransform.GetModelMtx();
		stl::FForEach(mChilds.begin(), mChilds.end(), [](GameObject* x) {x->UpdateWorldTransform(); });
	}

	namespace {
		constexpr const char* s_common_str[] = { "Components", "Name", "Transform", "Childs" };
		constexpr const size_t s_Components_idx = 0, s_Name_idx = 1, s_Transform_idx = 2,
			s_Childs_idx = 3;
	}

	// ------------------------------------------------------------------------
	/*! To Json
	*
	*   Serializes a Game Object to a Json File
	*/ //----------------------------------------------------------------------
	void GameObject::ToJson(json& val) const noexcept {
		size_t _count = 0;
		json& _refcomponents = val[s_common_str[s_Components_idx]];
		json& _refchilds = val[s_common_str[s_Childs_idx]];

		stl::FForEach(mChilds.begin(), mChilds.end(), [&_refchilds, &_count](GameObject* x) {
			json& ref = _refchilds[std::to_string(_count)];
			x->ToJson(ref), ref[s_common_str[s_Name_idx]] = x->GetName();
			});

		_count = 0;

		stl::FForEach(mComponents.begin(), mComponents.end(), [&_refcomponents, &_count](Component* x) {
			json& ref = _refcomponents[std::to_string(_count++)];
			x->ToJson(ref);
			ref[s_common_str[s_Name_idx]] = x->GetName();
			});

		mLocalTransform.ToJson(val[s_common_str[s_Transform_idx]]), val[s_common_str[s_Name_idx]] = GetName();
	}

	// ------------------------------------------------------------------------
	/*! From Json
	*
	*   Retrieves a Game Object from a Json file
	*/ //----------------------------------------------------------------------
	void GameObject::FromJson(const json& val) noexcept {
		mLocalTransform.FromJson(val[s_common_str[s_Transform_idx]]);
		SetName(val[s_common_str[s_Name_idx]].get<std::string>().c_str());
		const json& _refcomponents = val[s_common_str[s_Components_idx]];
		const json& _refchilds = val[s_common_str[s_Childs_idx]];

		stl::FForEach(_refcomponents.begin(), _refcomponents.end(),
			[this](const json& a) {
				Component* _a = reinterpret_cast<Component*>(
					FactoryManager::Instance().Create(a[s_common_str[s_Name_idx]].get<std::string>().c_str()));
				_a->mOwner = this;
				_a->FromJson(a);
				_a->SetName(a["Name"].get<std::string>().c_str());
				AddComp(_a);
			});

		stl::FForEach(_refchilds.begin(), _refchilds.end(), [this](const json& a) {
			//try to retrieve the component
			GameObject* temp = AddChild(
				a[s_common_str[s_Name_idx]].get<std::string>().c_str());
			temp->FromJson(a);
			});

		UpdateWorldTransform();
	}

	// ------------------------------------------------------------------------
	/*! Has Tag
	*
	*   Returns wether a Base Class has a tag
	*/ //--------------------------------------------------------------------
	const bool GameObject::HasTag(const char* flg) const noexcept {
		//Traverse through all tags
		for (auto& x : mTags)
			//If we have the same flag
			if (!strcmp(x, flg))
				return true;
		return false;
	}

	// ------------------------------------------------------------------------
	/*! Remove tag
	*
	*   Removes a tag from the base class
	*/ //--------------------------------------------------------------------
	STATIC GameObject::RemoveTag(const char* flg) noexcept {
		const stl::Array<const char*>::iterator _end = mTags.end();

		//for every tag
		for (stl::Array<const char*>::iterator it = mTags.begin();
			it != _end; ++it)

			//If we can find an identical flag
			if (!strcmp(*it, flg)) {
				mTags.remove(*it);

				return;
			}
	}

	// ------------------------------------------------------------------------
	/*! Destroy
	*
	*   Destroys the GameObject, it's children and it's components
	*/ //--------------------------------------------------------------------
	void GameObject::Destroy() {
		stl::FForEach(mChilds.begin(), mChilds.end(), [](GameObject* x) { x->Destroy();  });
		stl::FForEach(mComponents.begin(), mComponents.end(), [](Component* x) { x->Destroy();  });
	}
}