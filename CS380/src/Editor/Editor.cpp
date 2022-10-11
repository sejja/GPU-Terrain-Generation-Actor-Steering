//
//	Editor.cpp
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright � 2021 Digipen. All Rights reserved
//

#include <Shared.h>

namespace BaleaEditor {
	// ------------------------------------------------------------------------
	/*! Initialize
	*
	*   Initializes ImGui and the Editor
	*/ // ---------------------------------------------------------------------
	void ImGuiManager::Initialize() {
		mEditorStates.push_back(new BaleaEditor::HeaderState);
		mEditorCamera = new BaleaEngine::EditorCamera();
		mEditorCamera->Move({ 25, 15, 20 });
		stl::FForEach(mEditorStates.begin(), mEditorStates.end(), [](BaleaEditor::IEditorState* x)
			{ x->Load(); });
	}

	// ------------------------------------------------------------------------
	/*! Process Input
	*
	*   Processes the Input
	*/ // ---------------------------------------------------------------------
	void ImGuiManager::ProcessInput()
	{
		// Start the Dear ImGui frame
		ShowGUI();
		ImGui::EndFrame();

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	// ------------------------------------------------------------------------
	/*! Shut Down
	*
	*   Destroys ImGui and it's context
	*/ // ---------------------------------------------------------------------
	void ImGuiManager::Shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
		if (mEditorCamera)
			delete mEditorCamera;
	}

	// ------------------------------------------------------------------------
	/*! New Scene
	*
	*   Creates a New Scene
	*/ // ---------------------------------------------------------------------
	void ImGuiManager::NewScene() noexcept
	{
		mCurrentScene = new BaleaEngine::Scene();
		BaleaEngine::GameObject* light = mCurrentScene->SpawnObject("Default Light");
		light->SetPosition(BaleaEngine::Math::Vector3D(2.0f, 1.0f, 5.0f));
		UNUSED(light->AddComponent<BaleaEngine::Graphics::Light>("Light"));
		BaleaEngine::GameObject* terrain = mCurrentScene->SpawnObject("Terrain");
		UNUSED(terrain->AddComponent<CarProj::Terrain>("Terrain"));
	}

	// ------------------------------------------------------------------------
	/*! Show GUI
	*
	*   Presents the Editor ImGui GUI
	*/ // ---------------------------------------------------------------------
	void ImGuiManager::ShowGUI() noexcept
	{
		Dockspace();
		stl::FForEach(mEditorStates.begin(), mEditorStates.end(), [](BaleaEditor::IEditorState* st) {
			//If the window is visible
			if (ImGui::BeginTabItem(st->GetName())) {
				auto state = st;

				state->PreDraw();
				state->Present();
				ImGui::EndTabItem();
			}
			});
		ImGui::EndTabBar();
		ImGui::End();
	}

	// ------------------------------------------------------------------------
	/*! DockSpace
	*
	*   Create the Dockspace Context
	*/ // ---------------------------------------------------------------------
	void ImGuiManager::Dockspace() noexcept
	{
		static bool truebool = true;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("_dockspace_", &truebool, window_flags);
		ImGui::BeginTabBar("Main_Tab_bar");
		ImGui::PopStyleVar();
		ImGui::PopStyleVar(2);
		ImGuiIO& io = ImGui::GetIO();
		ImGuiID dockspace_id = ImGui::GetID("_dockspace_");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));
	}
}