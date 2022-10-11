//
//	Component.h
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 Digipen. All Rights reserved
//

#ifndef _COMPONENT__H_
#define _COMPONENT__H_

namespace BaleaEngine {
	struct GameObject;

	struct Component : public Serializable, public IBase
	{
#pragma region // Declarations & Properties
		friend struct GameObject;
#pragma endregion

#pragma region // Constructors & Destructors
		Component() noexcept;
		~Component() noexcept override;
#pragma endregion

#pragma region // Functions
		virtual void OnGui();
		GameObject inline* GetOwner() const noexcept;
#pragma endregion

#pragma region // Members
	private:
		GameObject* mOwner;
#pragma endregion
	};

	// ------------------------------------------------------------------------
	/*! Get Owner
	*
	*   Returns the Owner of the Component
	*/ // ----------------------------------------------------------------------
	GameObject inline* Component::GetOwner() const noexcept {
		return mOwner;
	}
}

#endif