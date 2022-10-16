//
//	RigidBody.cpp
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 . All Rights reserved
//

#include <Shared.h>

namespace BaleaEngine {
	namespace Physics {
		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*   Adds the RigidBody to the Physics system
		*/ // ---------------------------------------------------------------------
		RigidBody::RigidBody() noexcept
			: mAccel(PhysicsMgr->GetGlobalGravity())
		{
			PhysicsMgr->AddRigidBody(this);
		}

		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*   Removes the RigidBody from the Physics system
		*/ // ---------------------------------------------------------------------
		RigidBody::~RigidBody() noexcept {
			PhysicsMgr->RemoveRigidBody(this);
		}

		// ------------------------------------------------------------------------
		/*! Initialize
		*
		*   EMPTY FUNCTION
		*/ // ---------------------------------------------------------------------
		void RigidBody::Initialize()
		{
		}

		// ------------------------------------------------------------------------
		/*! Update
		*
		*   EMPTY FUNCTION
		*/ // ---------------------------------------------------------------------
		void RigidBody::Update()
		{
		}

		// ------------------------------------------------------------------------
		/*! On Gui
		*
		*   Adds Visuals to Edit the RigidBody Component (scoped)
		*/ // ---------------------------------------------------------------------
		void RigidBody::OnGui() noexcept {
			BaleaEditor::UIPropertyDraw _UIDraw;

			UNUSED(_UIDraw << std::make_pair("Add Force", &mVelocity));
		}

		namespace {
			constexpr const char* s_common_str[] = { "RigidBody", "Velocity" };
			constexpr const size_t s_RigidBody_idx = 0, s_Velocity_idx = 1;
		}

		// ------------------------------------------------------------------------
		/*! From Json
		*
		*   Saves the RigidBody to a Json
		*/ // ---------------------------------------------------------------------
		void RigidBody::FromJson(const json& val) noexcept
		{
			mVelocity = Serializers::MathSerializer::Vector3DFromJson(val[s_common_str[s_RigidBody_idx]]
				[s_common_str[s_Velocity_idx]]);
		}

		// ------------------------------------------------------------------------
		/*! To Json
		*
		*   Retrieves the RigidBody from a Json
		*/ // ---------------------------------------------------------------------
		void RigidBody::ToJson(json& val) const noexcept
		{
			val[s_common_str[s_RigidBody_idx]]
				[s_common_str[s_Velocity_idx]] = Serializers::MathSerializer::Vector3DToJson(mVelocity);
		}

		// ------------------------------------------------------------------------
		/*! Integrate
		*
		*   Adds the RigidBody to the Physics system
		*/ // ---------------------------------------------------------------------
		void RigidBody::Integrate(const float deltaTime) noexcept
		{
			mVelocity += mAccel * deltaTime;
			GetOwner()->SetPosition(GetOwner()->GetPosition() + mVelocity * deltaTime + mAccel * deltaTime * deltaTime / 2.0f);
		}
	}
}