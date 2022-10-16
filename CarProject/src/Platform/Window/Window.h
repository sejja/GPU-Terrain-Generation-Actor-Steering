//
//	Window.h
//	Balea Engine
//
//	Created by Diego Revilla on 09/09/20
//	Copyright © 2021 . All Rights reserved
//

#ifndef _WINDOW__H_
#define _WINDOW__H_

namespace BaleaEngine {
	namespace Platform {
		class  Window {
#pragma region // Methods
		public:
			bool Initialize();
			STATIC inline Close() noexcept;
			STATIC Update() noexcept;
			bool CreateWin32WindowClass(const char* name);
#pragma endregion

#pragma region // Constructors & Destructors
		protected:
			Window() noexcept;
			Window(const Window&) = delete;
			Window(Window&&) = delete;
#pragma endregion

#pragma region // Members
		private:
			HWND mWinHandle;
			HINSTANCE mInstance;
			LPCSTR mWinClassName;
			Math::Vector2D mPosition;
			const char* mTitle;
			bool mFullscreen;
			bool mExists;
			Math::Vector2D mDimensions;
			long mWindowStyle;
#pragma endregion

#pragma region // Gettors & Settors
		public:
			DONTDISCARD decltype(mWinHandle) inline GetHandle() const noexcept;
			DONTDISCARD decltype(mExists) inline Exists() const noexcept;
			DONTDISCARD decltype(mPosition) inline GetPosition() const noexcept;
			DONTDISCARD decltype(mDimensions) inline GetDimensions() const noexcept;
			DONTDISCARD decltype(mFullscreen) inline GetFullscreen() const noexcept;
			DONTDISCARD inline decltype(mTitle) GetTitle() const noexcept;
			DONTDISCARD decltype(mWindowStyle) inline GetStyle() const noexcept;
			STATIC GetCurrentMonitorSize(int& left, int& top, int& right, int& bottom) const noexcept;
			static Window& GetInstance() noexcept;
			DONTDISCARD HINSTANCE inline GetWindowInstance() const noexcept;
			STATIC SetPosition(const decltype(mPosition)& vec) noexcept;
			STATIC SetSize(const decltype(mDimensions)& vec) noexcept;
			STATIC SetTitle(decltype(mTitle) title) noexcept;
			STATIC SetFullscreen(decltype(mFullscreen) enabled) noexcept;
#pragma endregion
		};

		// ------------------------------------------------------------------------
		/*! Get Window Instance
		*
		*   Returns an instance of the Window class (Win32 API)
		*/ // --------------------------------------------------------------------
		HINSTANCE inline Window::GetWindowInstance() const noexcept {
			return mInstance;
		}

		// ------------------------------------------------------------------------
		/*! Get Handle
		*
		*   Gets the Window Handle
		*/ // --------------------------------------------------------------------
		decltype(Window::mWinHandle) inline Window::GetHandle() const noexcept {
			return mWinHandle;
		}

		// ------------------------------------------------------------------------
		/*! Exists
		*
		*   Does the Window Exist?
		*/ // --------------------------------------------------------------------
		decltype(Window::mExists) inline Window::Exists() const noexcept {
			return mExists;
		}

		// ------------------------------------------------------------------------
		/*! Get Position
		*
		*   Get the Window Position
		*/ // --------------------------------------------------------------------
		decltype(Window::mPosition) inline Window::GetPosition() const noexcept {
			return mPosition;
		}

		// ------------------------------------------------------------------------
		/*! Initialize
		*
		*   Initializes the Window from our engine
		*/ // --------------------------------------------------------------------
		Math::Vector2D inline Window::GetDimensions() const noexcept {
			return mDimensions;
		}

		// ------------------------------------------------------------------------
		/*! Get Full Screen
		*
		*   Are we on fullscreen?
		*/ // --------------------------------------------------------------------
		decltype(Window::mFullscreen) inline Window::GetFullscreen() const noexcept {
			return mFullscreen;
		}

		// ------------------------------------------------------------------------
		/*! Get Title
		*
		*   Gets the Title of our Window
		*/ // --------------------------------------------------------------------
		inline decltype(Window::mTitle) Window::GetTitle() const noexcept {
			return mTitle;
		}

		// ------------------------------------------------------------------------
		/*! Get Style
		*
		*   Gets the Window Style Format
		*/ // --------------------------------------------------------------------
		decltype(Window::mWindowStyle) inline Window::GetStyle() const noexcept {
			return mWindowStyle;
		}

		// ------------------------------------------------------------------------
		/*! Close
		*
		*   Closes the Engine window
		*/ // --------------------------------------------------------------------
		STATIC Window::Close() noexcept {
			mExists = false;
		}
	}
}

#define WindowMgr	(&BaleaEngine::Platform::Window::GetInstance())

#endif