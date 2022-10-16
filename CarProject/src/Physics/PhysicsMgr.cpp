//
//	PhysicsMgr.cpp
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright � 2021 . All Rights reserved
//

#include <Shared.h>

namespace BaleaEngine {
	namespace Physics {
		// ------------------------------------------------------------------------
		/*! Update
		*
		*   Integrates every Rigid Body on the Physics Manager
		*/ // ---------------------------------------------------------------------
		void PhysicsManager::Update() const noexcept
		{
			stl::FForEach(mRigidBodies.begin(), mRigidBodies.end(), [](RigidBody* rb) {
				rb->Integrate(mTime->deltaTime);
				});
		}
	}
}