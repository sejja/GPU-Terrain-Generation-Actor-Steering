//
//	CollisionMgr.h
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright � 2021 . All Rights reserved
//

#ifndef _COLLISION_MGR__H_
#define _COLLISION_MGR__H_

namespace BaleaEngine {
	namespace Physics {
		class CollisionManager {
		#pragma region // Definitions
			Singleton(CollisionManager);
		#pragma endregion

			void AddCollider(Collider* coll) {
				mColliders.push_back(coll);
			}

			void RemoveCollider(Collider* coll) {
				mColliders.remove(coll);
			}

		#pragma region // Members
		private:
			stl::Array <Collider*> mColliders;
		#pragma endregion

		public:
			decltype(mColliders)& GetColliders() {
				return mColliders;
			}

		};
	}
}

#define CollisionMgr (&BaleaEngine::Physics::CollisionManager::Instance())

#endif