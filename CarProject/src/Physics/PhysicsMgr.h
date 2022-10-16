//
//	PhysicsMgr.h
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright � 2021 . All Rights reserved
//

#ifndef _PHYSICS_MGR__H_
#define _PHYSICS_MGR__H_

namespace BaleaEngine {
	namespace Physics {
		class PhysicsManager
		{
#pragma region // Declarations
			Singleton(PhysicsManager);
#pragma endregion

#pragma region // Members
		private:
			Math::Vector3D mGlobalGravity;
			stl::Array<RigidBody*> mRigidBodies;
#pragma endregion

#pragma region // Methods
		public:
			void inline Initialize() noexcept;
			void Update() const noexcept;
			void inline AddRigidBody(decltype(mRigidBodies)::value_type rb) noexcept;
			void inline RemoveRigidBody(decltype(mRigidBodies)::value_type rb) noexcept;
			decltype(mGlobalGravity) inline GetGlobalGravity() const noexcept;
#pragma endregion
		};

		// ------------------------------------------------------------------------
		/*! Initialize
		*
		*   Sets the Gravity to it's original value
		*/ // ---------------------------------------------------------------------
		void inline PhysicsManager::Initialize() noexcept
		{
			mGlobalGravity = { 0.f, -9.81f, 0.f };
		}

		// ------------------------------------------------------------------------
		/*! Add Rigid Body
		*
		*   Adds a RigibBody to the Physics System
		*/ // ---------------------------------------------------------------------
		void inline PhysicsManager::AddRigidBody(
			decltype(mRigidBodies)::value_type rb) noexcept {
			mRigidBodies.push_back(rb);
		}

		// ------------------------------------------------------------------------
		/*! Remove Rigid Body
		*
		*   Removes a RigibBody from the Physics System
		*/ // ---------------------------------------------------------------------
		void inline PhysicsManager::RemoveRigidBody(
			decltype(mRigidBodies)::value_type rb) noexcept {
			mRigidBodies.remove(rb);
		}

		// ------------------------------------------------------------------------
		/*! Get Global Gravity
		*
		*   Returns the Gravity used by the Physics Manager
		*/ // ---------------------------------------------------------------------
		decltype(PhysicsManager::mGlobalGravity) inline 
			PhysicsManager::GetGlobalGravity() const noexcept {
			return mGlobalGravity;
		}
	}
}

#define PhysicsMgr (&BaleaEngine::Physics::PhysicsManager::Instance())

#endif