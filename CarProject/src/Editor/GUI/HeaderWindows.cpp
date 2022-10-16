//
//	HeaderWindows.cpp
//	Balea Engine
//
//	Created by Diego Revilla on 23/09/20
//	Copyright © 2021 . All Rights reserved
//

#include <Shared.h>

namespace BaleaEditor {
	// ------------------------------------------------------------------------
	/*! Default Constructor
	*
	*   Creates a Console Window
	*/ // ---------------------------------------------------------------------
	ConsoleWindow::ConsoleWindow() noexcept {
		memset(mInputBuf, NULL, sizeof(mInputBuf));
	}

	// ------------------------------------------------------------------------
	/*! Destructor
	*
	*   Destroys the Editor Console
	*/ // --------------------------------------------------------------------
	ConsoleWindow::~ConsoleWindow() noexcept {
		ClearLog();
	}

	// ------------------------------------------------------------------------
	/*! Clear Log
	*
	*   Clears the Log of the console
	*/ // --------------------------------------------------------------------
	void ConsoleWindow::ClearLog() noexcept {
		stl::allocator<char> alloc;

		stl::FForEach(mItems.begin(), mItems.end(), [&alloc](char* x) {
			alloc.deallocate(x);
		});

		mItems.clear();
	}

	// ------------------------------------------------------------------------
	/*! Text Edit Callback Stub
	*
	*   ImGui Dependency
	*/ // --------------------------------------------------------------------
	int ConsoleWindow::TextEditCallbackStub(ImGuiInputTextCallbackData* data) noexcept {
		return 0;
	}

	// ------------------------------------------------------------------------
	/*! Add Log
	*
	*   Adds an Item to the console
	*/ // --------------------------------------------------------------------
	void ConsoleWindow::AddLog(const char* str) noexcept {
		stl::allocator<char> alloc;
		char* nstr =  alloc.allocate(strlen(str));
		strcpy(nstr, str);
		mItems.push_back(nstr);
		ImGui::SetScrollHereY(1.f);
	}

	void ConsoleWindow::Present() noexcept {
		const float _ftheight = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
		bool reclaimfocus = false;

		//if we haven't created the console window
		if (ImGui::Begin("Console")) {
			ImGui::BeginChild("ScrollingRegion", ImVec2(0, -_ftheight), false,
				ImGuiWindowFlags_HorizontalScrollbar);
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

			//draw every single item stored
			for (char* x : mItems) {
				bool pop_color = false;

				//if it's an error
				if (strstr(x, "ERROR")) {
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
					pop_color = true;
				}

				ImGui::TextUnformatted(x);

				//if we need to color the text
				if (pop_color)
					ImGui::PopStyleColor();
			}

			if (mScrollToBottom)
				ImGui::SetScrollHereY(1.f);

			ImGui::PopStyleVar();
			ImGui::EndChild();
			ImGui::Separator();
			mScrollToBottom = false;

			//If we have inputed a command
			if (ImGui::InputText("Input Command", mInputBuf, IM_ARRAYSIZE(mInputBuf),
				ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion |
				ImGuiInputTextFlags_CallbackHistory, &TextEditCallbackStub, (void*)this)) {
				//If the command is valid
				if (mInputBuf) {
					AddLog(mInputBuf);

					//if the command is indeed clear
					if (!strcmp(mInputBuf, "clear"))
						ClearLog();
					else {
						bool found = false;

						//Loop through all the commands
						for (BaleaEditor::Console::Command* x : BaleaEditor::Console::_private_::_commands_) {
							std::string cmd_(mInputBuf);

							//If we find a match
							if (cmd_.rfind(x->command, 0) == 0) {
								size_t pos = cmd_.find(x->command);

								//If we can extract the arguments
								if (pos != std::string::npos)
									cmd_.erase(pos, strlen(x->command));

								x->Process(cmd_.c_str());
								found = true;
								break;
							}
						}

						//If we could not find the command
						if (!found)
							AddLog("ERROR: Command not found");
					}
				}

				strcpy_s(mInputBuf, 1, "");
				reclaimfocus = true;
			}

			//If we do have backlogs to print into the console
			if (!BaleaEditor::Console::_private_::_output_backlog_strs_.empty()) {
				do {
					AddLog(BaleaEditor::Console::_private_::_output_backlog_strs_.back().c_str());
					BaleaEditor::Console::_private_::_output_backlog_strs_.pop_back();
					mScrollToBottom = true;

					//do it while we still have strings to show
				} while (!BaleaEditor::Console::_private_::_output_backlog_strs_.empty());
			}

			ImGui::SetItemDefaultFocus();

			//If we should reclaim the focus of the HUD
			if (reclaimfocus)
				ImGui::SetKeyboardFocusHere(-1);

			ImGui::End();
		}
	}

	// ------------------------------------------------------------------------
	/*! Present
	*
	*   Profiles the FPS on a window
	*/ // ---------------------------------------------------------------------
	void FPSProfiler::Present() noexcept {
		//If we could create the window
		if (ImGui::Begin("Performance")) {
			PlotVar("FPS", 1.f / mTime->deltaTime, 0, 60);
			ImGui::End();
		}
	}

	// ------------------------------------------------------------------------
	/*! Defaiult Constructor
	*
	*   Sets the File Explorer to the right path
	*/ // ---------------------------------------------------------------------
	ExplorerWindow::ExplorerWindow() noexcept :
		mFileBrowser(File_browser("File Explorer", "data/")) {
	}

	// ------------------------------------------------------------------------
	/*! Present
	*
	*   Presents the content explorer
	*/ // ---------------------------------------------------------------------
	void ExplorerWindow::Present() noexcept {
		//If we choose a new path
		if (mFileBrowser.Present(mPath)) {
			nlohmann::json j_;
			std::ifstream input(mPath);

			input >> j_;
			input.close();
			//Editor->mCurrentScene->mObjectList.clear();
			Editor->GetCurrentScene()->FromJson(j_);
			//reinterpret_cast<ExplorerWindow*>(
				//reinterpret_cast<BaleaEditor::HeaderState*>(GetOwner())->GetWindow(3))->mCurrentfile = mPath;
		}
	}

	// ------------------------------------------------------------------------
	/*! Get Path
	*
	*   Returns the Path of the Explorer Window
	*/ // ---------------------------------------------------------------------
	std::string ExplorerWindow::GetPath() noexcept {
		return mPath;
	}

	// ------------------------------------------------------------------------
	/*! Get Current File
	*
	*   Gets the File of an Explorer
	*/ // ---------------------------------------------------------------------
	std::string&& ExplorerWindow::GetCurrentFile() noexcept {
		return std::move(mCurrentfile);
	}

	// ------------------------------------------------------------------------
	/*! Set Path
	*
	*   Sets the Path of the explorer
	*/ // ---------------------------------------------------------------------
	void ExplorerWindow::SetPath(const std::string& file) noexcept {
		mPath = file;
	}

	// ------------------------------------------------------------------------
	/*! Set Current File
	*
	*   Sets the File seleted by the Explorer Window
	*/ // ---------------------------------------------------------------------
	void ExplorerWindow::SetCurrentFile(const std::string& file) noexcept {
		mCurrentfile = file;
	}

	// ------------------------------------------------------------------------
	/*! Present
	*
	*   Presents the Importer Modal
	*/ // ---------------------------------------------------------------------
	void ImporterWindowModal::Present() noexcept {
		ExplorerWindow* ewin = reinterpret_cast<ExplorerWindow*>(
			reinterpret_cast<BaleaEditor::HeaderState*>(GetOwner())->GetWindow(3));

		std::string _temp = "data/";

		//If we enter a valid file
		if (fileBrowser.Present(true, _temp)) {
			nlohmann::json outjson;

			Editor->GetCurrentScene()->ToJson(outjson);

			std::ofstream output(_temp);
			output << outjson;

			output.close();

			//ewin->SetCurrentFile(_temp);
			reinterpret_cast<BaleaEditor::HeaderState*>(GetOwner())->RemoveWindow(this);
		}
	}

	// ------------------------------------------------------------------------
	/*! Present
	*
	*   Presents a Resource Window
	*/ // --------------------------------------------------------------------
	void ResourceWindow::Present() noexcept {
		//If we could show the Streamed Resources window
		/*if (ImGui::Begin("Streamed Resources")) {
			auto& map = ResourceManager::Instance().mAllResources;

			ImGui::Columns(4, "mycolumns");
			ImGui::Separator();
			ImGui::Text("Name"); ImGui::NextColumn();
			ImGui::Text("Full Path"); ImGui::NextColumn();
			ImGui::Text("Type"); ImGui::NextColumn();
			ImGui::Text("Size"); ImGui::NextColumn();

			//For every asset
			for (auto& x : map) {
				std::string_view a(x.first);

				a.remove_prefix(x.first.find_last_of("/\\") + 1);
				ImGui::Text(a.data()); ImGui::NextColumn();
				ImGui::Text(x.first.c_str()); ImGui::NextColumn();
				ImGui::Text(x.second->type); ImGui::NextColumn();
				ImGui::Text(x.second->size.c_str()); ImGui::NextColumn();
			}

			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::End();
		}*/
	}
}