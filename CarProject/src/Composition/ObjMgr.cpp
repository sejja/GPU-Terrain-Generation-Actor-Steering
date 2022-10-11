//
//	ObjMgr.cpp
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 Digipen. All Rights reserved
//

#include <Shared.h>

namespace BaleaEngine {
	namespace _private_ {
		// ------------------------------------------------------------------------
		/*! Update
		*
		*   Updates the Entire Scene
		*/ //----------------------------------------------------------------------
		void ObjectManager::Update() const {
			auto& temp_ = Editor->GetCurrentScene()->mObjectList;

			stl::FForEach(temp_.begin(), temp_.end(), [](BaleaEngine::GameObject* obj) {
				size_t counter = obj->GetChildCount(), i = 0;

				//While there are children to update
				while (i < counter)
					obj->GetChild(i++)->Update();

				//While there are components to update
				for (i = 0, counter = obj->GetComponentCount(); i < counter;)
					obj->GetComponent(i++)->Update();
				});
		}

		// ------------------------------------------------------------------------
		/*! Initialize Dispatch
		*
		*   Calls Initialize on unitilized objects
		*/ //----------------------------------------------------------------------
		void ObjectManager::InitializeDispatch() {
			stl::FForEach(beginqueue.begin(), beginqueue.end(), [](IBase* x) {
				x->Initialize();
				});
			beginqueue.clear();
		}
	}
}