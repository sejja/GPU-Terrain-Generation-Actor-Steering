//
//	InputMgr.h
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 . All Rights reserved
//

#ifndef _INPUT_MGR__H_
#define _INPUT_MGR__H_

namespace BaleaEngine {
	class InputManager {
#pragma region // Declarations
		Singleton(InputManager)

			using KeyCode = decltype(VK_ACCEPT);
		using NDCPointComponent = decltype(POINT::x);
#pragma endregion

#pragma region // Methods
		DONTDISCARD bool isKeyPressed(const KeyCode key) noexcept;
		DONTDISCARD bool isKeyReleased(const KeyCode key) noexcept;
		DONTDISCARD bool inline isKeyDown(const KeyCode key) const noexcept;
		DONTDISCARD bool inline isKeyUp(const KeyCode key) const noexcept;
		DONTDISCARD bool inline isButtonPressed(const KeyCode key) noexcept;
		DONTDISCARD bool inline isButtonReleased(const KeyCode key) noexcept;
		DONTDISCARD bool inline isButtonDown(const KeyCode key) const noexcept;
		DONTDISCARD bool inline isButtonUp(const KeyCode key) const noexcept;
		DONTDISCARD Math::Vector2D GetMouseMovement() const noexcept;
		DONTDISCARD Math::Vector2D inline GetMousePosition() const noexcept;
		void inline SetMousePosition(const Math::Vector2D& pos) const noexcept;
		void SetMousePosition(const int w, const int h) const noexcept;
		void ProcessInput() noexcept;
#pragma endregion

#pragma region // Members
	private:
		static constexpr size_t NumerOfKeys = 256;
		bool mKeyboardState[NumerOfKeys];
		Math::Vector2D mMousePositon;
#pragma endregion
	};

	// ------------------------------------------------------------------------
	/*! Is Key Down
	*
	*   Name says everything
	*/ // ---------------------------------------------------------------------
	bool inline InputManager::isKeyDown(const KeyCode key) const noexcept {
		return (GetAsyncKeyState(key) & (1 << 16));
	}

	// ------------------------------------------------------------------------
	/*! Is Key Up
	*
	*   Name says everything
	*/ // ---------------------------------------------------------------------
	bool inline InputManager::isKeyUp(const KeyCode key) const noexcept {
		return !isKeyDown(key);
	}

	// ------------------------------------------------------------------------
	/*! Is Button Pressed
	*
	*   Name says everything
	*/ // ---------------------------------------------------------------------
	bool inline InputManager::isButtonPressed(const KeyCode key) noexcept {
		return isKeyPressed(key);
	}

	// ------------------------------------------------------------------------
	/*! Is Button Released
	*
	*   Name says everything
	*/ // ---------------------------------------------------------------------
	bool inline InputManager::isButtonReleased(const KeyCode key) noexcept {
		return isKeyReleased(key);
	}

	// ------------------------------------------------------------------------
	/*! Is Button Down
	*
	*   Name says everything
	*/ // ---------------------------------------------------------------------
	bool inline InputManager::isButtonDown(const KeyCode key) const noexcept {
		return isKeyDown(key);
	}

	// ------------------------------------------------------------------------
	/*! Is Button Up
	*
	*   Name says everything
	*/ // ---------------------------------------------------------------------
	bool inline InputManager::isButtonUp(const KeyCode key)const noexcept {
		return isKeyUp(key);
	}

	// ------------------------------------------------------------------------
	/*! Get Mouse Position
	*
	*   Name says everything
	*/ // ---------------------------------------------------------------------
	Math::Vector2D inline InputManager::GetMousePosition() const noexcept {
		return mMousePositon;
	}

	// ------------------------------------------------------------------------
	/*! Set Mouse Position
	*
	*   Name says everything
	*/ // ---------------------------------------------------------------------
	void inline InputManager::SetMousePosition(const Math::Vector2D& pos) const noexcept {
		SetMousePosition(static_cast<NDCPointComponent>(pos.x), static_cast<NDCPointComponent>(pos.y));
	}
}

#define InputMgr (&BaleaEngine::InputManager::Instance())

#endif