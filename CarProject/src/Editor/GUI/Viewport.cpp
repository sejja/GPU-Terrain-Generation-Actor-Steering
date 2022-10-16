//
//	Viewport.cpp
//	Balea Engine
//
//	Created by Diego Revilla on 20/10/20
//	Copyright © 2021 . All Rights reserved
//

#include <Shared.h>

using namespace glm;

namespace BaleaEditor {
	namespace {
		constexpr const char* s_common_strs[] = { "UI\\Play.png", "Viewport", "T", "R", "S",
			"Input Modal" };
		constexpr const unsigned char sPlayT = 0, sViewportS = 1, sTS = 2, sRS = 3, sSS = 4,
			sModalS = 5;
		ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
		ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
	}

	// ------------------------------------------------------------------------
	/*! Initialize
	*
	*   Initializes the viewport and creates its own framebuffer
	*/ // ---------------------------------------------------------------------
	void Viewport::Initialize() noexcept {
		glGenFramebuffers(1, &mFramebufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, mFramebufferID);
		glGenTextures(1, &mRenderedTexture);
		glBindTexture(GL_TEXTURE_2D, mRenderedTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, static_cast<int>(mDimensions.x),
			static_cast<int>(mDimensions.y), 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glGenRenderbuffers(1, &mDepthrenderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, mDepthrenderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, static_cast<int>(mDimensions.x),
			static_cast<int>(mDimensions.y));
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthrenderbuffer);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mRenderedTexture, 0);
		unsigned DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		mPlayTex = ResourceMgr->Load<BaleaEngine::Graphics::Texture>("data/Texture/Play.png")->get();
		ImGuizmo::Enable(true);
		ImGuizmo::SetOrthographic(false);
	}

	// ------------------------------------------------------------------------
	/*! Decompose TransformE
	*
	*   Decomposes a Matrix intro separate Transform vectors
	*/ // --------------------------------------------------------------------
	static bool DecomposeTransform(const glm::mat4& transform,
		BaleaEngine::Math::Vector3D& translation, BaleaEngine::Math::Vector3D& rotation, 
		BaleaEngine::Math::Vector3D& scale) noexcept {
		glm::mat4 _LocalMatrix(transform);
		constexpr const float _epsilon = glm::epsilon<float>();

		// Normalize the matrix.
		if (!stl::Approx(_LocalMatrix[3][3], 0.f, _epsilon)) {
			// First, isolate perspective.  This is the messiest.
			if (!stl::Approx(_LocalMatrix[0][3], 0.f, _epsilon) ||
				!stl::Approx(_LocalMatrix[1][3], 0.f, _epsilon) ||
				!stl::Approx(_LocalMatrix[2][3], 0.f, _epsilon)) {
				_LocalMatrix[0][3] = _LocalMatrix[1][3] = _LocalMatrix[2][3] = 0.f;
				_LocalMatrix[3][3] = 1.f;
			}

			translation = BaleaEngine::Math::Vector3D(_LocalMatrix[3]);
			_LocalMatrix[3] = vec4(0, 0, 0, _LocalMatrix[3].w);

			vec3 _Row[3];

			// Now get scale and shear.
			for (length_t i = 0; i < 3; ++i)
				for (length_t j = 0; j < 3; ++j)
					_Row[i][j] = _LocalMatrix[i][j];
			// Compute X scale factor and normalize first row.
			scale.x = glm::length(_Row[0]);
			_Row[0] = glm::detail::scale(_Row[0], 1.f);
			scale.y = glm::length(_Row[1]);
			_Row[1] = glm::detail::scale(_Row[1], 1.f);
			scale.z = glm::length(_Row[2]);
			_Row[2] = glm::detail::scale(_Row[2], 1.f);

			rotation.y = stl::ASin(-_Row[0][2]);

			//If we have a non identity rotation
			if (stl::Cos(rotation.y) != 0) {
				rotation.x = atan2f(_Row[1][2], _Row[2][2]);
				rotation.z = atan2f(_Row[0][1], _Row[0][0]);
			}
			else {
				rotation.x = atan2f(-_Row[2][0], -_Row[2][0]);
				rotation.z = 0;
			}

			return true;
		}

		return false;
	}

	// ------------------------------------------------------------------------
	/*! Edit Transform
	*
	*   Decomposes a Matrix intro separate Transform vectors
	*/ // --------------------------------------------------------------------
	void EditTransform(const BaleaEngine::Camera& camera, glm::mat4& matrix) {
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y,
			ImGui::GetWindowWidth() - 15, ImGui::GetWindowHeight() - 35);
		ImGuizmo::Manipulate(glm::value_ptr(camera.getViewMtx()),
			glm::value_ptr(camera.getProjMtx()),
			mCurrentGizmoOperation, mCurrentGizmoMode, glm::value_ptr(matrix), NULL, NULL);
	}

	// ------------------------------------------------------------------------
	/*! Render
	*
	*   Renders the Viewport
	*/ // --------------------------------------------------------------------
	void Viewport::Present() {
		//If we can display the viewport
		if (ImGui::Begin(s_common_strs[sViewportS])) {
			//If we can display the toolset menu
			if (ImGui::Begin("Toolset")) {
				ImGui::Spacing();
				ImGui::Indent(ImGui::GetWindowWidth() / 2);

				//If we are currently playing
				if (mIsPlaying) {
					//IF we have to stop the gameplay
					if (ImGui::ImageButton(reinterpret_cast<void*>(static_cast<long long>(
						mPlayTex->GetGLHandle())), { 16, 16 }, { 0,0 }, { 1, 1 }, 8,
						{ 0, 0, 0, 0 }, { 0, 1, 0, 1 })) {
						Editor->SetSelectedObject(nullptr);
						mIsPlaying = !mIsPlaying;
						ResetSceneToPreviousScene();
						Editor->SetPlayingState(false);
					}
					ImGui::SameLine();
				}
				else {
					//If pressed play
					if (ImGui::ImageButton(reinterpret_cast<void*>(static_cast<long long>(
						mPlayTex->GetGLHandle())), { 16, 16 }, { 0,0 }, { 1, 1 }, 8,
						{ 0, 0, 0, 0 }, { 1, 1, 1, 1 })) {
						mIsPlaying = !mIsPlaying;
						RegisterSceneState();
						Editor->SetPlayingState(true);
						Editor->SetSelectedObject(nullptr);

						int count = Editor->GetCurrentScene()->GetObjectCount();
						for (int i = 0; i < count; i++) {
							auto ptr = Editor->GetCurrentScene()->GetObjectByID(i);
							
							auto temp = ptr->GetComponentByType<BaleaEngine::EditorCamera>();

							if (temp) {
								Editor->GetCurrentScene()->mViewCamera = temp;
								break;
							}
						}

						ImGui::End();
						ImGui::End();
						return;
					}
				}
			}

			ImGui::SameLine();
			ImGui::PushItemWidth(90);

			const char* _items[] = { s_common_strs[sViewportS], "Attached Window" };
			static const char* _current_item = _items[0];

			//If the combo is expanded
			if (ImGui::BeginCombo("##combo", _current_item)) {
				//Show every opetion
				for (int n = 0; n < sizeof(_items) / sizeof(const char*); n++) {
					bool _is_selected = (_current_item == _items[n]);

					//If option selected
					if (ImGui::Selectable(_items[n], _is_selected)) {
						_current_item = _items[n];
						mViewportselection = n;
					}

					//Reset
					if (_is_selected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			ImGui::PopItemWidth();
			ImGui::Unindent(ImGui::GetWindowWidth() / 2);
			ImGui::End();
		}

		const float _windowWidth = ImGui::GetWindowWidth() - 15;
		const float _windowHeight = ImGui::GetWindowHeight() - 35;
		mDimensions = { _windowWidth, _windowHeight };
		ImGui::Image(reinterpret_cast<void*>(static_cast<long long>(this->mRenderedTexture)),
			{ _windowWidth, _windowHeight }, { 0.f, -0.07f }, { 0.93f, -1.f }); //you haven't seen anything...

		//If we are in Editor mode
		if (!mIsPlaying) {
			ImGui::SetNextWindowPos({ ImGui::GetWindowPos().x + 15,
				ImGui::GetWindowPos().y + 35 }, ImGuiCond_Always, { 0.f, 0.f });
			bool _p_open = true;
			ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
			if (ImGui::Begin("Viewport Options", &_p_open, ImGuiWindowFlags_NoDecoration |
				ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
				ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav |
				ImGuiWindowFlags_NoMove)) {
				//If Selected Translation Option
				if (ImGui::Button(s_common_strs[sTS]))
					mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
				ImGui::SameLine();

				//If we want to rotate
				if (ImGui::Button(s_common_strs[sRS]))
					mCurrentGizmoOperation = ImGuizmo::ROTATE;
				ImGui::SameLine();

				//If we want to scale
				if (ImGui::Button(s_common_strs[sSS]))
					mCurrentGizmoOperation = ImGuizmo::SCALE;

				ImGui::Separator();
				ImGui::Text("Camera");

				int _temp = Editor->GetCameraSpeed();

				if (ImGui::SliderInt("Speed:", &_temp, 1, 9))
					Editor->SetCameraSpeed(_temp);
				ImGui::End();
			}
		}

		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y,
			_windowWidth, _windowHeight);

		//If we have a current base
		if (!Editor->isPlaying() && Editor->GetSelectedObject()) {
			auto _base = reinterpret_cast<BaleaEngine::GameObject*>(Editor->GetSelectedObject());
			auto _transform = _base->GetTransform().GetModelMtx();

			EditTransform(*reinterpret_cast<BaleaEngine::CameraComp*>(
				Editor->GetEditorCamera()), _transform);

			//If edited
			if (ImGuizmo::IsUsing()) {
				BaleaEngine::Math::Vector3D _translation, _rotation, _scale;
				DecomposeTransform(_transform, _translation, _rotation, _scale);
				BaleaEngine::GameObject* _base = reinterpret_cast<BaleaEngine::GameObject*>(Editor->GetSelectedObject());

				_base->SetPosition(_translation);
				//_base->SetRotation(_rotation);
				//_base->SetScale(_scale);
			}
		}

		//If we clicked inside the viewport
		if (ImGui::GetIO().MouseClicked[1]) {
			//Clear every popup
			if ((*GImGui).OpenPopupStack.size() > 0)
				(*GImGui).OpenPopupStack.pop_back();

			ImGui::OpenPopup(s_common_strs[sModalS]);
		}

		//Draw the Modal
		if (ImGui::BeginPopup(s_common_strs[sModalS])) {
			ImGui::Text("Transform: ");
			ImGui::Separator();

			//If we want to translate
			if (ImGui::Button(s_common_strs[sTS]))
				mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
			ImGui::SameLine();

			//If we want to rotate
			if (ImGui::Button(s_common_strs[sRS]))
				mCurrentGizmoOperation = ImGuizmo::ROTATE;
			ImGui::SameLine();

			//If we want to scale
			if (ImGui::Button(s_common_strs[sSS]))
				mCurrentGizmoOperation = ImGuizmo::SCALE;

			//lest manipulate the object directly
			if (ImGui::BeginMenu("Selected Object")) {
				//Want to delete the object?
				if (ImGui::MenuItem("Delete")) {
					Editor->GetCurrentScene()->RemoveObject(reinterpret_cast<BaleaEngine::GameObject*>(Editor->GetSelectedObject()));
					Editor->SetSelectedObject(nullptr);
				}

				ImGui::EndMenu();
			}

			ImGui::EndPopup();
		}

		ImGui::End();
	}

	// ------------------------------------------------------------------------
	/*! Register Scene State
	*
	*   Register previews scene state
	*/ // --------------------------------------------------------------------
	STATIC Viewport::RegisterSceneState() noexcept {
		mJ.clear();
		Editor->GetCurrentScene()->ToJson(mJ);
	}

	// ------------------------------------------------------------------------
	/*! Reset Scene to previous scene
	*
	*   Resets the scene to its previous state
	*/ // --------------------------------------------------------------------
	STATIC Viewport::ResetSceneToPreviousScene() noexcept {
		Editor->GetCurrentScene()->FromJson(mJ);
	}
}