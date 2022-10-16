//
//	ObjMgr.h
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 . All Rights reserved
//

#ifndef _OBJ_MGR__H_
#define _OBJ_MGR__H_

namespace BaleaEngine {
	struct IBase;
	struct GameObject;

	namespace _private_ {
		class ObjectManager {
#pragma region // Declarations
			Singleton(ObjectManager);

			friend struct GameObject;
			friend class ImGuiManager;
#pragma endregion

		public:

#pragma region // Functions
			void Update() const;
			void InitializeDispatch();
#pragma endregion

#pragma region // Members
			stl::Array<IBase*> beginqueue;
#pragma endregion
		};
	}
}

#define ObjMgr (&BaleaEngine::_private_::ObjectManager::Instance())

#endif