//
//	Collider.cpp
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 Digipen. All Rights reserved
//

#include <Shared.h>

namespace BaleaEngine {
	namespace Physics {
		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*   Sets the Collider Type to Sphere
		*/ // ---------------------------------------------------------------------
		Collider::Collider() noexcept :
			mColliderType(eColliderType::e_Sphere) {
			CollisionMgr->AddCollider(this);
		}

		// ------------------------------------------------------------------------
		/*! Conversion Constructor
		*
		*   Takes the Collider Type to be using
		*/ // ---------------------------------------------------------------------
		Collider::Collider(eColliderType colliderType) noexcept :
			mColliderType(colliderType) {
		}

		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*   EMPTY FUNCTION
		*/ // ---------------------------------------------------------------------
		Collider::~Collider() noexcept {
			CollisionMgr->RemoveCollider(this);
		}

		// ------------------------------------------------------------------------
		/*! Initialize
		*
		*   EMPTY FUNCTION
		*/ // ---------------------------------------------------------------------
		void Collider::Initialize() noexcept
		{
		}

		// ------------------------------------------------------------------------
		/*! Update
		*
		*   EMPTY FUNCTION
		*/ // ---------------------------------------------------------------------
		void Collider::Update() noexcept {
		}

		namespace {
			constexpr const char* s_common_str[] = { "Collider Type" };
			constexpr const size_t s_Collider_type_idx = 0;
		}

		// ------------------------------------------------------------------------
		/*! On Gui
		*
		*   Shows the Properties of the collider onto the Screen
		*/ // ---------------------------------------------------------------------
		void Collider::OnGui() noexcept {
			BaleaEditor::UIPropertyDraw UIDraw;

			UNUSED(UIDraw << std::make_tuple(s_common_str[s_Collider_type_idx],
				reinterpret_cast<int*>(&mColliderType), "Box\0Sphere\0\0"));
			ImGui::DragFloat("Radious", &mRadious, 1.0f, 0.1, 5);
		}

		// ------------------------------------------------------------------------
		/*! From Json
		*
		*   Retrieves the Component from a Json File (scoped)
		*/ // ---------------------------------------------------------------------
		void Collider::FromJson(const json& val) noexcept
		{
			mColliderType = static_cast<eColliderType>(val[s_common_str[s_Collider_type_idx]].get<int>());
		}

		// ------------------------------------------------------------------------
		/*! To Json
		*
		*   Saves the Component onto a Json FIle (scoped)
		*/ // ---------------------------------------------------------------------
		void Collider::ToJson(json& val) const noexcept
		{
			UNUSED(val[s_common_str[s_Collider_type_idx]] = static_cast<int>(mColliderType));
		}
	}
}