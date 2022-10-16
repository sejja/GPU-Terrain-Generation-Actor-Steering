//
//	Scene.cpp
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright � 2021 . All Rights reserved
//

#include <Shared.h>

namespace BaleaEngine {
	namespace {
		static stl::PageAllocator<Scene> gscenealloc;
		stl::PageAllocator<GameObject> gobjalloc;
	}

	// ------------------------------------------------------------------------
	/*! Default Constructor
	*
	*   Constructs an Scene
	*/ // ---------------------------------------------------------------------
	Scene::Scene() noexcept {
		auto scene = Editor->GetCurrentScene();

		//If there is already an existing scene
		if (scene) {
			gscenealloc.destroy(scene);
			gscenealloc.deallocate(scene);
		}
		scene = this;
	}

	// ------------------------------------------------------------------------
	/*! Destructor
	*
	*   Destroys the scene to ensure no memory leaking
	*/ // ---------------------------------------------------------------------
	Scene::~Scene() noexcept
	{
		Clear();
	}

	// ------------------------------------------------------------------------
	/*! Spawn Object
	*
	*   Spawns an Object in the Scene with the given name
	*/ // --------------------------------------------------------------------
	GameObject* Scene::SpawnObject(const char* name) {
		GameObject* obj_ = gobjalloc.allocate();
		gobjalloc.construct(obj_);
		obj_->SetName(name ? name : "No Name");
		mObjectList.push_back(obj_);

		return obj_;
	}

	// ------------------------------------------------------------------------
	/*! Spawn Object
	*
	*   Spawns an already created object onto the scene
	*/ // --------------------------------------------------------------------
	GameObject* Scene::SpawnObject(GameObject* go) noexcept {
		mObjectList.push_back(go);

		return go;
	}

	// ------------------------------------------------------------------------
	/*! Get Object By Name
	*
	*   Retrieves an Object by it's name
	*/ // ---------------------------------------------------------------------
	GameObject* Scene::GetObjectByName(const char* name) const noexcept {
		for (GameObject* x : mObjectList) {
			if (!strcmp(x->GetName(), name))
				return x;
		}
		return nullptr;
	}

	// ------------------------------------------------------------------------
	/*! Remove Object
	*
	*   Removes an Object From the Scene
	*/ // ---------------------------------------------------------------------
	void Scene::RemoveObject(GameObject* torem) const noexcept {
		GCollector::Instance().AddObjectToRemove(torem);
	}

	// ------------------------------------------------------------------------
	/*! From JSON
	*
	*   Retrieves a Scene from a Json Files
	*/ // --------------------------------------------------------------------
	void Scene::FromJson(const json& val) {
		Clear();
		stl::FForEach(val.begin(), val.end(), [this](const json& a) {
			GameObject* temp = SpawnObject();
			temp->FromJson(a);
			});
	}

	// ------------------------------------------------------------------------
	/*! To JSON
	*
	*   Saves the scene onto a Json File
	*/ // --------------------------------------------------------------------
	void Scene::ToJson(json& val) const {
		size_t count = 0;
		stl::FForEach(mObjectList.begin(), mObjectList.end(), [&val, &count](GameObject* x) {
			x->ToJson(val[std::to_string(count++)]);
			});
	}

	// ------------------------------------------------------------------------
	/*! Clear
	*
	*   Clears a Level from it's objects
	*/ // --------------------------------------------------------------------
	void Scene::Clear() noexcept {
		stl::FForEach(mObjectList.begin(), mObjectList.end(), [](BaleaEngine::GameObject* obj) {
			gobjalloc.destroy(obj);
			gobjalloc.deallocate(obj);
			});

		mObjectList.clear(), GfxMgr->ClearLights();
	}

	// ------------------------------------------------------------------------
	/*! Get Object Count
	*
	*   Get the Root Objects placed on the Scene
	*/ // --------------------------------------------------------------------
	size_t Scene::GetObjectCount() const noexcept {
		return mObjectList.size();
	}

	// ------------------------------------------------------------------------
	/*! Get Object By ID
	*
	*   Gets an Object based on it's ID
	*/ // --------------------------------------------------------------------
	GameObject* Scene::GetObjectByID(const size_t idx) const noexcept {
		return mObjectList[idx];
	}
}