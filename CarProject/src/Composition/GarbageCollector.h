//
//	GarbageCollector.h
//	Balea Engine
//
//	Created by Diego Revilla on 10/12/21
//	Copyright © 2021 Digipen. All Rights reserved
//

#ifndef _GARBAGE_COLLECTOR__H_
#define _GARBADE_COLLECTOR__H_

namespace BaleaEngine {
	struct GameObject;
	struct Component;

	class GCollector {
#pragma region // Declarations
		Singleton(GCollector)
#pragma endregion

#pragma region // Functions
	public:
		STATIC GOGarbageCollection();
		STATIC AddComponentToRemove(Component* comp);
		STATIC AddChildToRemove(GameObject* child);
		STATIC AddObjectToRemove(GameObject* obj);

#pragma endregion

#pragma region // Members
	private:
		stl::Array<Component*> mToRemoveComps;
		stl::Array<GameObject*> mToRemoveChilds;
		stl::Array<GameObject*> mToRemoveObjs;
#pragma endregion
	};
}

#endif
