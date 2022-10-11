//
//	Editor.h
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright � 2021 Digipen. All Rights reserved
//

#ifndef _EDITOR__H_
#define _EDITOR__H_

namespace BaleaEngine {
	struct GameObject;
	struct Scene;
}

namespace BaleaEditor {
	class ImGuiManager
	{
		enum class ETransform : unsigned short {
			ETranslate,
			ERotate,
			EScale
		};

		Singleton(ImGuiManager)

			void Initialize();
		void ProcessInput();
		void Shutdown();
		void NewScene() noexcept;
		void ShowGUI() noexcept;
		void Dockspace() noexcept;
		void inline SetPlayingState(const bool playState) noexcept;
		DONTDISCARD bool inline isPlaying() const noexcept;
		DONTDISCARD BaleaEngine::Scene inline* GetCurrentScene() const noexcept;
		DONTDISCARD BaleaEngine::GameObject inline* GetSelectedObject() const noexcept;
		void SetSelectedObject(BaleaEngine::GameObject* obj) noexcept;
		DONTDISCARD ETransform inline GetCurrentTransformOperation() const noexcept;
		void inline SetCurrentTransformOperation(const ETransform type) noexcept;
		void inline SetCameraSpeed(const int cameraspeed) noexcept;
		DONTDISCARD int inline GetCameraSpeed() const noexcept;
		void inline AddState(BaleaEditor::IEditorState* state) noexcept;
		DONTDISCARD GLuint inline GetViewportBuffer() const noexcept;

	private:
		bool mbIsPlaying = false;
		bool mbDockWindowOpen;
		BaleaEngine::Scene* mCurrentScene = nullptr;
		BaleaEngine::GameObject* mSelectedObject = nullptr;
		ImGuiDockNodeFlags mDockspaceFlags = ImGuiDockNodeFlags_None;
		ETransform mCurrentTransformOp = ETransform::ETranslate;
		int mCameraSpeed = 3;
		stl::Array<BaleaEditor::IEditorState*> mEditorStates;
		BaleaEngine::EditorCamera* mEditorCamera;

	public:
		DONTDISCARD decltype(mEditorCamera) inline GetEditorCamera() const noexcept;
	};

	// ------------------------------------------------------------------------
	/*! Set Playing State
	*
	*   Sets if the scene is playing or not
	*/ // ---------------------------------------------------------------------
	void inline BaleaEditor::ImGuiManager::SetPlayingState(const bool playState) noexcept {
		mbIsPlaying = playState;
	}

	// ------------------------------------------------------------------------
	/*! Is Playing
	*
	*   Gets wether we are on play state or not
	*/ // ---------------------------------------------------------------------
	bool inline ImGuiManager::isPlaying() const noexcept {
		return  mbIsPlaying;
	}

	// ------------------------------------------------------------------------
	/*! Get Current Scene
	*
	*   Gets the current Scene
	*/ // ---------------------------------------------------------------------
	BaleaEngine::Scene inline* ImGuiManager::GetCurrentScene() const noexcept {
		return mCurrentScene;
	}

	// ------------------------------------------------------------------------
	/*! Get Selected Object
	*
	*   Returns the Selected Object
	*/ // ---------------------------------------------------------------------
	BaleaEngine::GameObject inline* ImGuiManager::GetSelectedObject() const noexcept {
		return mSelectedObject;
	}

	// ------------------------------------------------------------------------
	/*! Set Selected Object
	*
	*   Sets the Object to Select
	*/ // ---------------------------------------------------------------------
	void inline ImGuiManager::SetSelectedObject(BaleaEngine::GameObject* obj) noexcept {
		mSelectedObject = obj;
	}

	// ------------------------------------------------------------------------
	/*! Set Camera Speed
	*
	*   Sets the Speed of the Editor Camera
	*/ // ---------------------------------------------------------------------
	void inline BaleaEditor::ImGuiManager::SetCameraSpeed(const int cameraspeed) noexcept {
		mCameraSpeed = cameraspeed;
	}

	// ------------------------------------------------------------------------
	/*! Get Camera Speed
	*
	*   Gets the Speed of the Camera
	*/ // ---------------------------------------------------------------------
	int inline ImGuiManager::GetCameraSpeed() const noexcept {
		return mCameraSpeed;
	}

	// ------------------------------------------------------------------------
	/*! Add State
	*
	*   Adds an State to the Editor
	*/ // ---------------------------------------------------------------------
	void inline ImGuiManager::AddState(BaleaEditor::IEditorState* state) noexcept {
		mEditorStates.push_back(state);
	}

	// ------------------------------------------------------------------------
	/*! Get Viewport Buffer
	*
	*   Gets the Buffer the Scene is rendered onto inside the Editor
	*/ // ---------------------------------------------------------------------
	GLuint inline ImGuiManager::GetViewportBuffer() const noexcept {
		return reinterpret_cast<BaleaEditor::Viewport*>(
			mEditorStates[0]->GetWindow(7))->GetBuffer();
	}

	// ------------------------------------------------------------------------
	/*! Get Editor Camera
	*
	*   Gets the Camera the Editor uses
	*/ // ---------------------------------------------------------------------
	decltype(ImGuiManager::mEditorCamera) inline ImGuiManager::GetEditorCamera() const noexcept {
		return mEditorCamera;
	}

	// ------------------------------------------------------------------------
	/*! Get Current Transform Operation
	*
	*   Returns what operation the Gizmos are doing
	*/ //--------------------------------------------------------------------
	ImGuiManager::ETransform inline ImGuiManager::GetCurrentTransformOperation() const noexcept {
		return mCurrentTransformOp;
	}

	// ------------------------------------------------------------------------
	/*! Set Current Transform Operation
	*
	*   Sets the Current Transform operation of the Gizmos
	*/ //--------------------------------------------------------------------
	void inline ImGuiManager::SetCurrentTransformOperation(const ETransform type) noexcept {
		mCurrentTransformOp = type;
	}
}

#define Editor (&BaleaEditor::ImGuiManager::Instance())

#endif