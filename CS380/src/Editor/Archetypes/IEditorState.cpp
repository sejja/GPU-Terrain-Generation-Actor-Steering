//
//	IEditorState.cpp
//	Balea Engine
//
//	Created by Diego Revilla on 21/11/20
//	Copyright © 2021 Digipen. All Rights reserved
//

#include <Shared.h>

namespace BaleaEditor {
	// ------------------------------------------------------------------------
	/*! Present
	*
	*   Presents all the windows of one sgtate
	*/ // ---------------------------------------------------------------------
	void IEditorState::Present() {
		stl::FForEach(mWindows.begin(), mWindows.end(), [](IWindow* x) {
			if (x->mActive)
				x->Present();
			});
	}

	// ------------------------------------------------------------------------
	/*! Add Window
	*
	*   Adds a window to the state
	*/ // --------------------------------------------------------------------
	STATIC IEditorState::AddWindow(IWindow* win) noexcept {
		win->mOwnerState = this;
		mWindows.push_back(win);
	}

	// ------------------------------------------------------------------------
	/*! Remove Window
	*
	*   Removes a window from the State
	*/ // ---------------------------------------------------------------------
	STATIC IEditorState::RemoveWindow(IWindow* win) noexcept {
		mWindows.remove(win);
		delete win;
	}

	// ------------------------------------------------------------------------
	/*! Destructor
	*
	*   Frees all the windows
	*/ // ---------------------------------------------------------------------
	IEditorState::~IEditorState() noexcept {
		stl::FForEach(mWindows.begin(), mWindows.end(), [](IWindow* x) { delete x; });
	}
}