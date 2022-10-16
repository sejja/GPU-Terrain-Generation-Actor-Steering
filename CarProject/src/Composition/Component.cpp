//
//	Component.cpp
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 . All Rights reserved
//

#include <Shared.h>

namespace BaleaEngine {
	// ------------------------------------------------------------------------
	/*! Default Constructors
	*
	*   Sets the Owner to nullptr
	*/ // ---------------------------------------------------------------------
	Component::Component() noexcept :
		mOwner(nullptr)
	{
	}

	// ------------------------------------------------------------------------
	/*! Destructor
	*
	*   EMPTY FUNCTION
	*/ // ---------------------------------------------------------------------
	Component::~Component() noexcept
	{
	}

	// ------------------------------------------------------------------------
	/*! On Gui
	*
	*   EMPTY FUNCTION
	*/ // ---------------------------------------------------------------------
	void Component::OnGui() {
	}
}