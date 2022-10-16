//
//	RigidBody.h
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 . All Rights reserved
//

#ifndef _RIGID_BODY__H_
#define _RIGID_BODY__H_

namespace BaleaEngine {
	namespace Physics {
		struct RigidBody : public Component {
#pragma region // Constructors & Destructors
			RigidBody() noexcept;
			~RigidBody() noexcept;
#pragma endregion

#pragma region // Functions
			void Initialize() override;
			void Update() override;
			void Integrate(const float deltaTime) noexcept;
			void OnGui() noexcept override;
			void FromJson(const json& val) noexcept override;
			void ToJson(json& val) const noexcept override;
			void inline SetVelocity(const Math::Vector3D& vel) noexcept;
			void inline SetAcceleration(const Math::Vector3D& acc) noexcept;
			DONTDISCARD Math::Vector3D inline GetVelocity() const noexcept;
			DONTDISCARD Math::Vector3D inline GetAcceleration() const noexcept;

#pragma endregion

#pragma region // Members
		private:
			Math::Vector3D mVelocity;
			Math::Vector3D mAccel;
#pragma endregion
		};

		// ------------------------------------------------------------------------
		/*! Set Velocity
		*
		*   Sets the Velocity of a RigidBody
		*/ // ---------------------------------------------------------------------
		void inline RigidBody::SetVelocity(const Math::Vector3D& vel) noexcept {
			mVelocity = vel;
		}

		// ------------------------------------------------------------------------
		/*! Set Acceleration
		*
		*   Sets the Acceleration of a RigidBody
		*/ // ---------------------------------------------------------------------
		void inline RigidBody::SetAcceleration(const Math::Vector3D& acc) noexcept {
			mAccel = acc;
		}

		// ------------------------------------------------------------------------
		/*! Get Velocity
		*
		*   Gets the Velocity of a RigidBody
		*/ // ---------------------------------------------------------------------
		Math::Vector3D inline RigidBody::GetVelocity() const noexcept {
			return mVelocity;
		}

		// ------------------------------------------------------------------------
		/*! Get Acceleration
		*
		*   Gets the acceleration of a RigidBody
		*/ // ---------------------------------------------------------------------
		Math::Vector3D inline RigidBody::GetAcceleration() const noexcept {
			return mAccel;
		}
	}
}

#endif