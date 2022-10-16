//
//	Scene.h
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 . All Rights reserved
//

#ifndef _SCENE__H_
#define _SCENE__H_

namespace BaleaEngine {
	namespace _private_ {
		class ObjectManager;
	}

	struct Scene : public Serializable {
#pragma region // Defines
		friend class GCollector;
		friend class _private_::ObjectManager;
#pragma endregion

#pragma region // Constructors & Destructors
		Scene() noexcept;
		~Scene() noexcept;
#pragma endregion

#pragma region // Functions
		void FromJson(const json& val);
		void ToJson(json& val) const;
		DONTDISCARD GameObject* SpawnObject(const char* name = nullptr);
		DONTDISCARD GameObject* SpawnObject(GameObject* go) noexcept;
		DONTDISCARD GameObject* GetObjectByName(const char* name) const noexcept;
		void RemoveObject(GameObject* torem) const noexcept;
		DONTDISCARD GameObject* GetObjectByID(const size_t idx) const noexcept;
		DONTDISCARD size_t GetObjectCount() const noexcept;
		void Clear() noexcept;
		EditorCamera* mViewCamera;
#pragma endregion

#pragma region // Members
	private:
		stl::Array<BaleaEngine::GameObject*> mObjectList;
#pragma endregion
	};
}

#endif