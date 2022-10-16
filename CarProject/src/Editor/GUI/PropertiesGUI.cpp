//
//	PropertiesGUI.cpp
//	Balea Engine
//
//	Created by Diego Revilla on 3/2/20
//	Copyright © 2021 . All Rights reserved
//

#include <Shared.h>

namespace BaleaEditor {
	namespace {
		constexpr const char* s_common_strs[] = { "Transform",
		"GameObject", "No Name", "Space" };
	}

#define eTransform  s_common_strs[0]
#define eGameObject s_common_strs[1]
#define eNoName     s_common_strs[2]
#define eSpace      s_common_strs[3]

	// ------------------------------------------------------------------------
	/*! Present
	*
	*   Draws the Properties Window
	*/ //----------------------------------------------------------------------
	void PropertiesWindow::Present() noexcept {
		UIPropertyDraw _UIDraw;
		BaleaEngine::GameObject* _base = reinterpret_cast<BaleaEngine::GameObject*>(Editor->GetSelectedObject());

		//Add the properties menu
		if (ImGui::Begin("Properties")) {
			//If we have a target object
			if (_base) {
				BaleaEngine::GameObject* const _obj = _base;

				static char _name[20];
				strcpy(_name, _base->GetName());
				if (ImGui::InputText(_base->GetName(), _name, 20, ImGuiInputTextFlags_EnterReturnsTrue))
					_base->SetName(_name);

				ImGui::Separator();

				//Create a child, called transform
				if (ImGui::BeginChild(eTransform)) {
					//If we want to display the transform
					if (ImGui::CollapsingHeader(eTransform)) {
						BaleaEngine::Math::Vector3D _temp_v3 = _base->GetPosition();

						if (_UIDraw << std::make_pair("Position", &_temp_v3))
							_base->SetPosition(_temp_v3);
						_temp_v3 = _base->GetScale();
						if (_UIDraw << std::make_pair("Scale", &_temp_v3))
							_base->SetScale(_temp_v3);

						_temp_v3 = _base->GetRotation();
						if (_UIDraw << std::make_pair("Rotation", &_temp_v3))
							_base->SetRotation(_temp_v3);
					}

					const size_t _ccount = _base->GetComponentCount();

					//For every Child in the object
					for (int y = 0; y < _ccount; y++) {
						BaleaEngine::Component* _comp = _base->GetComponent(y);
						std::string _name = _comp->GetName();

						ImGui::Separator();
						ImGui::PushID((void*)(intptr_t)_comp);

						//If we expand the property
						if (ImGui::CollapsingHeader(_name.c_str())) {
							//If we display the options palette
							if (ImGui::BeginMenu("Options")) {
								//Remove object?
								if (ImGui::MenuItem("Remove Component")) {
									_obj->RemoveComp(_comp);
									ImGui::EndMenu();
									ImGui::PopID();
									ImGui::EndChild();
									ImGui::End();

									return;
								}

								ImGui::EndMenu();
							}

							_comp->OnGui();
						}

						ImGui::PopID();
					}

					ImGui::EndChild();
				}
			}
			else
				ImGui::Text("To view properties, please select an object from the scene");
			ImGui::End();
		}
	}

	// ------------------------------------------------------------------------
	/*! Present
	*
	*   Presents the Palette Window
	*/ //----------------------------------------------------------------------
	void SceneWindow::Present() noexcept {
		//If we can present the Scene Inspector
		if (ImGui::Begin("Scene")) {
			//For each space
			ImGui::Columns(3, "mycolumns");
			ImGui::Separator();
			ImGui::Text("ID"); ImGui::NextColumn();
			ImGui::Text("Name"); ImGui::NextColumn();
			ImGui::Text("Type"); ImGui::NextColumn();

			for (size_t i = 0, size = Editor->GetCurrentScene()->GetObjectCount();
				i < size; i++) {
				static char _label[256];
				sprintf(_label, "%03llu", i);

				BaleaEngine::GameObject* _y = Editor->GetCurrentScene()->GetObjectByID(i);

				//If we select the gameObject
				if (ImGui::Selectable(_label, _y == Editor->GetSelectedObject(),
					ImGuiSelectableFlags_SpanAllColumns)) {
					Editor->SetSelectedObject(_y);
				}

				ImGui::NextColumn();
				ImGui::Text(_y->GetName());
				ImGui::NextColumn();
				ImGui::Text(eGameObject);
				ImGui::NextColumn();
			}

			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::TreePop();
		}

		ImGui::End();
	}

	// ------------------------------------------------------------------------
	/*! Present
	*
	*   Presents the Palette Window
	*/ //----------------------------------------------------------------------
	void PaletteWindow::Present() noexcept {
		//If we can show the Tools window
		if (ImGui::Begin("Tools")) {
			//If we can show the Objects
			if (ImGui::BeginTabBar("Objects")) {
				const ImVec2 _buttonsize(200, 30);

				//And we are on the Basic Tab
				if (ImGui::BeginTabItem("Basic")) {
					if (ImGui::Button("Transform", _buttonsize))
						BaleaEngine::GameObject* pObj = Editor->GetCurrentScene()->SpawnObject("Unnamed");
					if (ImGui::Button("Cube", _buttonsize)) {
						BaleaEngine::GameObject* pObj = Editor->GetCurrentScene()->SpawnObject("Unnamed");
						dynamic_cast<BaleaEngine::Graphics::MeshRenderer*>(
							pObj->AddComponent<BaleaEngine::Graphics::MeshRenderer>("MeshRenderer")
							)->SetModel("data/Model/Default Shapes/Cube.obj");
					}
					if (ImGui::Button("Sphere", _buttonsize)) {
						BaleaEngine::GameObject* pObj = Editor->GetCurrentScene()->SpawnObject("Unnamed");
						dynamic_cast<BaleaEngine::Graphics::MeshRenderer*>(
							pObj->AddComponent<BaleaEngine::Graphics::MeshRenderer>("MeshRenderer")
							)->SetModel("data/Model/Default Shapes/Sphere.obj");
					}
					if (ImGui::Button("Plane", _buttonsize)) {
						BaleaEngine::GameObject* pObj = Editor->GetCurrentScene()->SpawnObject("Unnamed");
						dynamic_cast<BaleaEngine::Graphics::MeshRenderer*>(
							pObj->AddComponent<BaleaEngine::Graphics::MeshRenderer>("MeshRenderer")
							)->SetModel("data/Model/Default Shapes/Plane.obj");
					}

					ImGui::EndTabItem();
				}

				//Begin the Tab for the C++ Classes
				if (ImGui::BeginTabItem("Components")) {
					if (ImGui::CollapsingHeader("Engine Classes")) {
						BaleaEngine::GameObject* const _base = Editor->GetSelectedObject();
						if (ImGui::Button("RigidBody", _buttonsize) && _base)
							Editor->GetSelectedObject()->AddComponent<BaleaEngine::Physics::RigidBody>("RigidBody");
						if (ImGui::Button("Collider", _buttonsize) && _base)
							Editor->GetSelectedObject()->AddComponent<BaleaEngine::Physics::Collider>("Collider");
						if (ImGui::Button("Light", _buttonsize) && _base)
							Editor->GetSelectedObject()->AddComponent<BaleaEngine::Graphics::Light>("Light");
					}

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}

			ImGui::End();
		}
	}
}