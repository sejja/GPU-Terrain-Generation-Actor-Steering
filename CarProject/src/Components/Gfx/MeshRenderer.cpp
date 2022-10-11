//
//	MeshRenderer.cpp
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 Digipen. All Rights reserved
//

#include <Shared.h>

namespace BaleaEngine {
	namespace Graphics {
		namespace fs = std::filesystem;

		// ------------------------------------------------------------------------
		/*! Mesh Renderer
		*
		*   Constructs a Mesh Renderer
		*/ // ---------------------------------------------------------------------
		MeshRenderer::MeshRenderer() noexcept {
			SetShader("data/Shaders/Default.shader");
		}

		namespace {
			constexpr const char* s_common_str[] = { "MeshRenderer", "Model", "None" };
			constexpr const size_t s_MeshRenderer_idx = 0, s_Model_idx = 1, s_None_idx = 2;
		}

		// ------------------------------------------------------------------------
		/*! On Gui
		*
		*   Draws the GUI for editing this Mesh Renderer component on the Editor (scoped)
		*/ // ---------------------------------------------------------------------
		void MeshRenderer::OnGui() noexcept
		{
			ImGui::NextColumn();

			const auto _fSeletcionEdit = [this](std::string& actualname, const char* title, const char* path,
				const char* extensions[], size_t extensionsize) {
					//Begin the Object Edition Combo
					if (ImGui::BeginCombo(title, actualname.c_str()))
					{
						fs::path _modelDir(path);

						int _count = 0;

						//For each entry on the directory
						for (auto dirIt = fs::recursive_directory_iterator(_modelDir);
							dirIt != fs::recursive_directory_iterator();
							dirIt++, ++_count)
						{
							//If it's a directory, skip it
							if (fs::is_directory(dirIt->path()))
								continue;

							const std::string _extension = dirIt->path().extension().string();

							//If it's not one of the extensions we want, skip it
							for (size_t i = 0; i < extensionsize; i++) {
								if (_extension == extensions[i]) {
									const std::string _relPath = dirIt->path().relative_path().string();
									const std::string _currentFileName = ResourceMgr->GetResourceName(_relPath.c_str()) + _extension;

									//If we selected one of the entries
									if (ImGui::Selectable((_currentFileName + "##" + std::to_string(_count)).c_str()))
									{
										actualname = _currentFileName;
										mModel = ResourceMgr->Load<Model>(_relPath.c_str());
									}

									break;
								}
							}
						}

						ImGui::EndCombo();
					}

					ImGui::NextColumn();
			};

			static const char* _extensions3d[] = { ".obj", ".fbx" };
			static std::string _modelActualName = mModel ? mModel->GetRelativePath() : s_common_str[s_None_idx];

			_fSeletcionEdit(_modelActualName, "##Model", "data/Model", _extensions3d, 2);

			static const char* _extensionsShader[] = { ".shader" };
			const auto _shader = GetShader();
			static std::string _shaderActualName = _shader ? _shader->GetRelativePath() : s_common_str[s_None_idx];

			_fSeletcionEdit(_modelActualName, "##Shader", "data/Shaders", _extensionsShader, 1);

			ImGui::Text("Material");

			if (ImGui::Button("Edit on ShaderGraph"))
				BaleaEditor::SelectGraph(&this->mModel->get()->mMaterial.mBlueprint);

		}

		// ------------------------------------------------------------------------
		/*! From Json
		*
		*   Retrieves the Mesh Renderer component from a Json File (scoped)
		*/ // ---------------------------------------------------------------------
		void MeshRenderer::FromJson(const json& val)  noexcept {
			mModel = ResourceMgr->Load<Model>(val[s_common_str[s_MeshRenderer_idx]]
				[s_common_str[s_Model_idx]]["Resource Path"].get<std::string>().c_str());
		}

		// ------------------------------------------------------------------------
		/*! To Json
		*
		*   Saves the Mesh Renderer component to a Json File (scoped)
		*/ // ---------------------------------------------------------------------
		void MeshRenderer::ToJson(json& val) const noexcept {
			mModel->ToJson(val[s_common_str[s_MeshRenderer_idx]]
				[s_common_str[s_Model_idx]]);
		}

		// ------------------------------------------------------------------------
		/*! Draw
		*
		*   Draws the Mesh Renderer
		*/ // ---------------------------------------------------------------------
		void MeshRenderer::Draw() noexcept {
			const auto shader_ = GetShader();

			//If we have a valid Model and Shader
			if (mModel && GetShader())
				mModel->get()->Draw(*shader_->get());
		}
	}
}