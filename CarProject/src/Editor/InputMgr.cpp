//
//	InputMgr.cpp
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 Digipen. All Rights reserved
//

#include <Shared.h>

namespace BaleaEngine {
	// ------------------------------------------------------------------------
	/*! Is Key Pressed
	*
	*   Name says everything
	*/ // ---------------------------------------------------------------------
	bool InputManager::isKeyPressed(const KeyCode key) noexcept {
		bool previousState = mKeyboardState[key];

		mKeyboardState[key] = isKeyDown(key);

		return (mKeyboardState[key] && !previousState);
	}

	// ------------------------------------------------------------------------
	/*! Is Key Released
	*
	*   Name says everything
	*/ // ---------------------------------------------------------------------
	bool InputManager::isKeyReleased(const KeyCode key) noexcept {
		bool previousState = mKeyboardState[key];

		mKeyboardState[key] = isKeyDown(key);

		return (!mKeyboardState[key] && previousState);
	}

	// ------------------------------------------------------------------------
	/*! Get Mouse Movement
	*
	*   Gets the mouse differential in position with relation of the previos frame
	*/ // ---------------------------------------------------------------------
	Math::Vector2D InputManager::GetMouseMovement() const noexcept {
		POINT p;
		GetCursorPos(&p);
		ScreenToClient(GetActiveWindow(), &p);

		Math::Vector2D pos(static_cast<float>(p.x), static_cast<float>(p.y));

		pos.x -= mMousePositon.x;
		pos.y -= mMousePositon.y;

		return pos;
	}

	// ------------------------------------------------------------------------
	/*! Set Mouse Position
	*
	*   Name says everything
	*/ // ---------------------------------------------------------------------
	void InputManager::SetMousePosition(const int w, const int h) const noexcept {
		POINT p;

		p.x = w;
		p.y = h;
		ClientToScreen(GetActiveWindow(), &p);
		SetCursorPos(p.x, p.y);
	}

	// ------------------------------------------------------------------------
	/*! Process Input
	*
	*   Updates the Input Information for the application
	*/ // ---------------------------------------------------------------------
	void InputManager::ProcessInput() noexcept {
		for (KeyCode i = 0; i < NumerOfKeys;)
			mKeyboardState[i] = isKeyDown(i++);

		POINT p;

		if (isButtonPressed(VK_ESCAPE)) exit(0);

		GetCursorPos(&p);
		ScreenToClient(GetActiveWindow(), &p);
		mMousePositon.x = static_cast<float>(p.x);
		mMousePositon.y = static_cast<float>(p.y);
	}
}