//
//	Header.cpp
//	Balea Engine
//
//	Created by Diego Revilla on 23/09/20
//	Copyright © 2021 . All Rights reserved
//

#include <Shared.h>

namespace BaleaEditor {
	// ------------------------------------------------------------------------
	/*! Render
	*
	*   Renders the Head of the Editor
	*/ //--------------------------------------------------------------------
	void HeaderState::PreDraw() noexcept {
		//If we can display the top bar
		if (ImGui::BeginMenuBar()) {
			ImGui::MenuItem("CarProj Engine");

			//IF we are in the File Menu
			if (ImGui::BeginMenu("File")) {
				//If we want to create a new scene
				if (ImGui::MenuItem("New Scene")) {
					Editor->GetCurrentScene()->Clear();
					Editor->SetSelectedObject(nullptr);
					reinterpret_cast<ExplorerWindow*>(GetWindow(3))->SetCurrentFile("");
				}

				//If we are not working on a file
				if (reinterpret_cast<ExplorerWindow*>(GetWindow(3))->GetCurrentFile().empty()) {
					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
					ImGui::PushStyleVar(ImGuiStyleVar_Alpha,
						ImGui::GetStyle().Alpha * 0.5f);
				}

				//If we want to save the current map
				if (ImGui::MenuItem("Save", "Ctrl+S")) {
					nlohmann::json savejson;
					Editor->GetCurrentScene()->ToJson(savejson);

					std::fstream output(reinterpret_cast<ExplorerWindow*>(
						reinterpret_cast<HeaderState*>(this)->GetWindow(3))->GetCurrentFile());

					output << savejson;

					output.close();
				}

				//If we are not working on a file
				if (reinterpret_cast<ExplorerWindow*>(GetWindow(3))->GetCurrentFile().empty()) {
					ImGui::PopItemFlag();
					ImGui::PopStyleVar();
				}

				//If we wanna save the scene
				if (ImGui::MenuItem("Save As...")) {
					ImporterWindowModal* temp = new ImporterWindowModal;

					temp->SetActive(true);
					AddWindow(temp);
				}

				ImGui::EndMenu();
			}

			//Create an Options Section
			if (ImGui::BeginMenu("View")) {
				static BaleaEditor::IWindow* defaultwins[] = { GetWindow(0),
					GetWindow(1), GetWindow(2), GetWindow(3), GetWindow(4) };

				//If we want to show the console
				if (ImGui::MenuItem("Show Console", nullptr, defaultwins[1]->GetActive()))
					defaultwins[0]->SetActive(!defaultwins[1]->GetActive());

				//If we want to show the project explorer
				if (ImGui::MenuItem("Show Project Explorer", nullptr, GetWindow(3)->GetActive()))
					defaultwins[0]->SetActive(!GetWindow(3)->GetActive());

				ImGui::Separator();

				//If we want to show the project explorer
				if (ImGui::MenuItem("Show Profiler", nullptr, defaultwins[1]->GetActive()))
					defaultwins[1]->SetActive(!defaultwins[1]->GetActive());

				//If we want to show the project explorer
				if (ImGui::MenuItem("Show Resource Window", nullptr, defaultwins[4]->GetActive()))
					defaultwins[0]->SetActive(!defaultwins[4]->GetActive());

				ImGui::Separator();

				//If we want to show the project explorer
				if (ImGui::MenuItem("Show World Settings", nullptr, defaultwins[0]->GetActive()))
					defaultwins[0]->SetActive(!defaultwins[0]->GetActive());

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
	}

	// ------------------------------------------------------------------------
	/*! Present
	*
	*   Presents the header
	*/ //--------------------------------------------------------------------
	void HeaderState::Present() noexcept {
		//For every window
		for (size_t i = 0, count = GetWindowCount(); i < count; i++) {
			BaleaEditor::IWindow* x = GetWindow(i);

			//If the window is active
			if (x->GetActive())
				x->Present();
		}
	}

	// ------------------------------------------------------------------------
	/*! Init Head
	*
	*   Initialized the Head
	*/ //--------------------------------------------------------------------
	void HeaderState::Load() noexcept {
		PropertiesWindow* tempproperties_ = new BaleaEditor::PropertiesWindow;
		SceneWindow* tempscene_ = new BaleaEditor::SceneWindow;
		PaletteWindow* temppalette_ = new BaleaEditor::PaletteWindow;
		ExplorerWindow* tempexplorer_ = new BaleaEditor::ExplorerWindow;
		Viewport* tempviewport_ = new Viewport;
		HumeWindow* temphume_ = new HumeWindow;

		SetName("Main Window");
		tempproperties_->SetActive(true);
		tempscene_->SetActive(true);
		temppalette_->SetActive(true);
		tempviewport_->SetActive(true);
		tempexplorer_->SetActive(true);
		tempexplorer_->SetPath("data/");
		temphume_->SetActive(true);
		AddWindow(new ConsoleWindow);
		AddWindow(new FPSProfiler);
		AddWindow(new ResourceWindow);
		AddWindow(tempproperties_);
		AddWindow(tempexplorer_);
		AddWindow(tempscene_);
		AddWindow(temppalette_);
		AddWindow(tempviewport_);
		AddWindow(temphume_);
		temphume_->Initialize();
		tempviewport_->Initialize();
	}
}