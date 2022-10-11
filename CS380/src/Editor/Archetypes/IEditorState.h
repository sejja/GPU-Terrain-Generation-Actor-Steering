//
//	IEditorState.h
//	Balea Engine
//
//	Created by Diego Revilla on 21/11/20
//	Copyright © 2021 Digipen. All Rights reserved
//

#ifndef _I_EDITOR_STATE__H_
#define _I_EDITOR_STATE__H_

namespace BaleaEditor {
	struct IWindow;

	struct IEditorState {
	public:

#pragma region // Constructors & Destructors
		~IEditorState() noexcept;
#pragma endregion

#pragma region // Functions
		VIRTUAL Load() = 0;
		VIRTUAL PreDraw() = 0;
		VIRTUAL Present();
		STATIC AddWindow(IWindow* win) noexcept;
		STATIC RemoveWindow(IWindow* win) noexcept;
		STATIC inline SetName(const char* str) noexcept;
		DONTDISCARD IWindow inline* GetWindow(const size_t idx) const noexcept;
		DONTDISCARD size_t inline GetWindowCount() const noexcept;
		DONTDISCARD const char inline* GetName() const noexcept;
#pragma endregion

#pragma region // Members
	private:
		std::string mName;
		stl::Array<IWindow*> mWindows;
#pragma endregion
	};

	// ------------------------------------------------------------------------
	/*! Open Level
	*
	*   Gets a window from the state given the index
	*/ // ---------------------------------------------------------------------
	DONTDISCARD IWindow inline* IEditorState::GetWindow(const size_t idx) const noexcept {
		return mWindows[idx];
	}

	// ------------------------------------------------------------------------
	/*! Get Window Count
	*
	*   Returns the Total Window Count
	*/ // ---------------------------------------------------------------------
	DONTDISCARD size_t inline IEditorState::GetWindowCount() const noexcept {
		return mWindows.size();
	}

	// ------------------------------------------------------------------------
	/*! Get Name
	*
	*   Returns the State Name
	*/ // ---------------------------------------------------------------------
	DONTDISCARD const char inline* IEditorState::GetName() const noexcept {
		return mName.c_str();
	}

	// ------------------------------------------------------------------------
	/*! Set Name
	*
	*   Sets the Name of the State
	*/ // ---------------------------------------------------------------------
	STATIC inline IEditorState::SetName(const char* str) noexcept {
		mName = str;
	}
}

#endif