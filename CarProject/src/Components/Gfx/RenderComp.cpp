//
//	RenderComp.cpp
//	Balea Engine
//
//	Created by Diego Revilla on 11/05/21
//	Copyright © 2021 . All Rights reserved
//

#include <Shared.h>

namespace BaleaEngine {
	namespace Graphics {
		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*   Adds this Renderable to the Render List
		*/ // ---------------------------------------------------------------------
		RenderComp::RenderComp() noexcept {
			GfxMgr->AddToRenderList(this);
		}

		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*   Removes this Renderable from the Render List
		*/ // ---------------------------------------------------------------------
		RenderComp::~RenderComp() noexcept {
			GfxMgr->RemoveFromRenderList(this);
		}
	}
}