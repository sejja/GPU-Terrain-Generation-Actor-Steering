//
//	Window.cpp
//	Balea Engine
//
//	Created by Diego Revilla on 09/09/20
//	Copyright � 2021 . All Rights reserved
//

#include <Shared.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler
(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace BaleaEngine {
	namespace Platform {
		LRESULT CALLBACK ProxyWinProc(
			_In_ HWND   hWnd,
			_In_ UINT   message,
			_In_ WPARAM wParam,
			_In_ LPARAM lParam
		);

		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*   EMPTY FUNCTION
		*/ // --------------------------------------------------------------------
		Window::Window() noexcept :
			mWinHandle(0), mInstance(0), mWinClassName("Balea Engine"), mPosition{ 0, 0 }, mTitle(nullptr), mFullscreen(false),
			mExists(true), mWindowStyle(WS_OVERLAPPEDWINDOW) {
		}

		// ------------------------------------------------------------------------
		/*! Get Instance
		*
		*   REturns a reference to the window Singleton
		*/ // --------------------------------------------------------------------
		Window& Window::GetInstance() noexcept {
			static Window mwin_;

			return mwin_;
		}

		// ------------------------------------------------------------------------
		/*! Create Win32 Window Class
		*
		*   Registers a new class on the OS
		*/ // --------------------------------------------------------------------
		bool Window::CreateWin32WindowClass(const char* name) {
			WNDCLASSEX winClass_;

			mInstance = GetModuleHandle(NULL);
			ZeroMemory(&winClass_, sizeof(WNDCLASSEX));
			winClass_.cbSize = sizeof(WNDCLASSEX);
			winClass_.style = CS_HREDRAW | CS_VREDRAW;
			winClass_.lpfnWndProc = (WNDPROC)ProxyWinProc;
			winClass_.hInstance = mInstance;
			winClass_.hIcon = LoadIcon(mInstance, IDI_APPLICATION);
			winClass_.hCursor = LoadCursor(NULL, IDC_ARROW);
			winClass_.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);
			winClass_.lpszClassName = name;
			winClass_.hIconSm = LoadIcon(winClass_.hInstance, IDI_APPLICATION);

			//If we could not register a new class
			if (!RegisterClassEx(&winClass_))
				throw(std::runtime_error("Failed To Create Class"));

			return true;
		}

		// ------------------------------------------------------------------------
		/*! Initialize
		*
		*   Initializes the Window from our engine
		*/ // --------------------------------------------------------------------
		bool Window::Initialize() {
			//If we could not register a class on our OS
			if (!CreateWin32WindowClass("Balea Engine"))
				return false;

			mWinHandle = CreateWindow(
				mWinClassName,
				TEXT("CarProj "),
				mWindowStyle,
				10, 10,
				1792, 1008,
				NULL,
				NULL,
				mInstance,
				NULL
			);

			SetSize({ 1792, 1008 });

			//If we do not have a valid handle
			if (!mWinHandle) {
				mExists = false;
				throw std::runtime_error("Failed to Create Win32 Window");
			}

			mExists = true;
			ShowWindow(mWinHandle, SW_SHOW);
			UpdateWindow(mWinHandle);
			SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);

			::ShowWindow(::GetConsoleWindow(), SW_HIDE);

			return true;
		}

		// ------------------------------------------------------------------------
		/*! Get Current Monitor Size
		*
		*   Gets the size of the current monitor the app runs in
		*/ // --------------------------------------------------------------------
		STATIC Window::GetCurrentMonitorSize(int& left, int& top, int& right,
			int& bottom) const noexcept {
			const HMONITOR currentMonitor_ = MonitorFromWindow(mWinHandle,
				MONITOR_DEFAULTTONEAREST);
			MONITORINFO info_;

			info_.cbSize = sizeof(MONITORINFO);
			GetMonitorInfo(currentMonitor_, &info_);
			left = info_.rcWork.left;
			top = info_.rcWork.top;
			right = info_.rcWork.right;
			bottom = info_.rcWork.bottom;
		}

		// ------------------------------------------------------------------------
		/*! Initialize
		*
		*   Initializes the Window from our engine
		*/ // --------------------------------------------------------------------
		STATIC Window::SetPosition(const decltype(mPosition)& vec) noexcept {
			mPosition = vec;
			SetWindowPos(mWinHandle, NULL, static_cast<int>(mPosition.x),
				static_cast<int>(mPosition.y), 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		}

		// ------------------------------------------------------------------------
		/*! Set Size
		*
		*   Sets the Size of a window
		*/ // --------------------------------------------------------------------
		STATIC Window::SetSize(const Math::Vector2D& vec) noexcept {
			mDimensions = vec;
			RECT metricRect_ = { 0, 0, static_cast<LONG>(mDimensions.x),
				static_cast<LONG>(mDimensions.y) };

			AdjustWindowRect(&metricRect_, mWindowStyle, FALSE);
			SetWindowPos(mWinHandle, NULL, 0, 0,
				metricRect_.right - metricRect_.left,
				metricRect_.bottom - metricRect_.top,
				SWP_NOZORDER | SWP_NOMOVE);
		}

		// ------------------------------------------------------------------------
		/*! Set Title
		*
		*   Sets the Title of the window
		*/ // --------------------------------------------------------------------
		STATIC Window::SetTitle(decltype(mTitle) title) noexcept {
			mTitle = title;
			SetWindowText(mWinHandle, mTitle);
		}

		// ------------------------------------------------------------------------
		/*! Set Fullscreen
		*
		*   Sets the app to go fullscreen
		*/ // --------------------------------------------------------------------
		STATIC Window::SetFullscreen(const decltype(mFullscreen) enabled) noexcept {
			//If we have changes
			if (mFullscreen != enabled) {
				mFullscreen = enabled;

				//If we wanna go fullscreen
				if (mFullscreen) {
					int left_, top_, bottom_, right_;
					GetCurrentMonitorSize(left_, top_, right_, bottom_);
					mWindowStyle = WS_POPUP;
					SetWindowLongPtr(mWinHandle, GWL_STYLE, mWindowStyle);
					SetWindowPos(mWinHandle, NULL, left_, top_, right_ - left_,
						bottom_ - top_, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
				}
				else {
					mWindowStyle = WS_OVERLAPPEDWINDOW;
					SetWindowLongPtr(mWinHandle, GWL_STYLE, mWindowStyle);
					SetWindowPos(mWinHandle, NULL, static_cast<int>(mPosition.x),
						static_cast<int>(mPosition.y), static_cast<int>(mDimensions.x),
						static_cast<int>(mDimensions.y), SWP_FRAMECHANGED | SWP_SHOWWINDOW);
				}
			}
		}

		// ------------------------------------------------------------------------
		/*! Update
		*
		*   Updates the Engine Window
		*/ // --------------------------------------------------------------------
		STATIC Window::Update() noexcept {
			MSG msg_;

			//while we still have to read OS messages
			while (PeekMessage(&msg_, NULL, NULL, NULL, PM_REMOVE)) {
				//Switch by the incoming message
				switch (msg_.message) {
					//case quit
				case WM_QUIT:
					mExists = false;
					break;

					//any other case
				default:
					TranslateMessage(&msg_);
					DispatchMessage(&msg_);
				}
			}
		}

		// ------------------------------------------------------------------------
		/*! Proxy Win Proc
		*
		*   This is how the OS Comunicates with us (really, or at least to
		*       the window), redirect to function
		*/ // --------------------------------------------------------------------
		LRESULT CALLBACK ProxyWinProc(_In_ HWND hWin, _In_ UINT msg,
			_In_ WPARAM wp, _In_ LPARAM lp) {
			if (ImGui_ImplWin32_WndProcHandler(
				hWin, msg, wp, lp))
				return true;

			//switch by the incoming message
			switch (msg) {
				//case destroy
			case WM_DESTROY:
				PostQuitMessage(0); break;
				break;

				//case create
			case WM_CREATE:
				return 0;

				//case resize
			case WM_SIZE:
			{
				const auto& _window = *WindowMgr;
				Math::Vector2D _dimensions = _window.GetDimensions();

				WindowMgr->SetSize({ static_cast<float>(LOWORD(lp)), static_cast<float>(HIWORD(lp)) });
				RECT r = { 0, 0, static_cast<LONG>(_dimensions.x),
					static_cast<LONG>(_dimensions.y) };
				::AdjustWindowRect(&r, _window.GetStyle(), FALSE);
			}

			break;

			default:
				return DefWindowProc(hWin, msg, wp, lp);
			}

			return 0;
		}
	}
}