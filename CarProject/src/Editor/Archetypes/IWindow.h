//
//	IWindow.h
//	Balea Engine
//
//	Created by Diego Revilla on 21/11/20
//	Copyright © 2021 . All Rights reserved
//

#ifndef _I_WINDOW__H_
#define _I_WINDOW__H_

namespace BaleaEditor {
	struct IEditorState;

	struct IWindow {
		friend IEditorState;

	public:
		IWindow();
		VIRTUAL Present() {};
		void inline SetActive(const bool b) noexcept;
		DONTDISCARD bool GetActive() const noexcept;
		DONTDISCARD IEditorState* GetOwner() const noexcept;
	protected:
		bool mActive = false;
		IEditorState* mOwnerState;
	};

	// ------------------------------------------------------------------------
	/*! Set Active
	*
	*   Flags the Window as Active
	*/ // ---------------------------------------------------------------------
	void inline IWindow::SetActive(const bool b) noexcept {
		mActive = b;
	}

	// ------------------------------------------------------------------------
	/*! Get Active
	*
	*   Gets wether the Window is Active or not
	*/ // ---------------------------------------------------------------------
	bool inline BaleaEditor::IWindow::GetActive() const noexcept {
		return mActive;
	}

	// ------------------------------------------------------------------------
	/*! Get Owner
	*
	*   Retrieves the Owner State of a Window
	*/ // ---------------------------------------------------------------------
	IEditorState inline* BaleaEditor::IWindow::GetOwner() const noexcept {
		return mOwnerState;
	}
}

#endif