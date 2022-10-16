//
//	FileExplorer.cpp
//	Balea Engine
//
//	Created by Diego Revilla on 31/10/20
//	Copyright © 2021 . All Rights reserved
//

#include <Shared.h>

namespace BaleaEditor {
	namespace {
		BaleaEngine::Graphics::Texture* s_Textures[7] = { nullptr };
		constexpr const unsigned char eFolderT = 0, eLevelT = 1, eSourceT = 2,
			eAudioT = 3, eSpaceT = 4, eAnimationT = 5, eUnknownT = 6;

		const char* s_common_strs[] = { "name", "path", "data\\Texture\\Folder.png", "data\\Texture\\Level.png",
			"data\\Texture\\Source.png", "data\\Texture\\Audio.png", "data\\Texture\\Space.png", "data\\Texture\\Bone.png", "data\\Texture\\Unknown.png",
			"meta" };
		constexpr const unsigned char eNameS = 0, ePathS = 1, eFolderS = 2, eLevelS = 3,
			eSourceS = 4, eAudioS = 5, eSpaceS = 6, eAnimationS = 7, eUnknownS = 8, eMetaS = 9;
	}

	// ------------------------------------------------------------------------
	/*! Get Files In Path
	*
	*   Get the files listed within a path
	*/ //--------------------------------------------------------------------
	static void get_files_in_path(const std::filesystem::path& path,
		stl::Array<File>& files) noexcept {
		files.clear();

		//If the path does not exist
		if (std::filesystem::exists(path)) {
			//If we have a parent path
			if (path.has_parent_path())
				files.push_back({ "..", path.parent_path() });

			//For every file in directory
			for (const auto& dirEntry : std::filesystem::directory_iterator(path)) {
				const std::filesystem::path& _dirPath = dirEntry.path();
				const std::string _metaf = _dirPath.filename().string();
				std::string _extension;
				size_t _idx = _metaf.find_last_of('.');

				//If we have an extension
				if (_idx != std::string::npos)
					_extension = _metaf.substr(_idx + 1);

				//If we have a path instead
				else if (std::filesystem::is_directory(_dirPath)) {
					files.push_back({ _dirPath.filename().string().c_str(), _dirPath,
						File::Filetype::eFolder, nullptr });
					continue;
				}
				else
					continue;

				//If the pointed file is a level
				if (!strcmp(_extension.c_str(), "blevel"))
					files.push_back({ _metaf, _dirPath,
						File::Filetype::eLevel, nullptr });

				//If the pointer file is a png
				else if (!strcmp(_extension.c_str(), "png"))
					files.push_back({ _metaf, _dirPath,
						File::Filetype::eTexture,
						ResourceMgr->Load<BaleaEngine::Graphics::Texture>
						(_metaf.c_str())->get() });

				//If the pointed file is a wav file
				else if (!strcmp(_extension.c_str(), "wav") ||
					!strcmp(_extension.c_str(), "mp3"))
					files.push_back({ _metaf, _dirPath,
						File::Filetype::eAudio, nullptr });

				//If the pointed file is a space file
				else if (!strcmp(_extension.c_str(), "aspace"))
					files.push_back({ _metaf, _dirPath,
						File::Filetype::eSpace, nullptr });

				//If the pointed file is a space file
				else if (!strcmp(_extension.c_str(), "atlas"))
					files.push_back({ _metaf, _dirPath,
						File::Filetype::eAnimation, nullptr });

				//If the pointed file is none of the above
				else
					files.push_back({ _metaf, _dirPath,
						File::Filetype::eUnknown, nullptr });
			}
		}
	}

	// ------------------------------------------------------------------------
	/*! Get Files In Path
	*
	*   Get the files listed within a path
	*/ //--------------------------------------------------------------------
	static void get_files_in_path_recursive(const std::filesystem::path& path,
		stl::Array<File>& files) noexcept {
		files.clear();

		//If the path does not exist
		if (std::filesystem::exists(path)) {
			//If we have a parent path
			if (path.has_parent_path())
				files.push_back({ "..", path.parent_path() });

			//For every file in directory
			for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(path)) {
				const std::filesystem::path& _dirPath = dirEntry.path();
				const std::string _metaf = _dirPath.filename().string();
				std::string _extension;
				size_t _idx = _metaf.find_last_of('.');

				//If we have an extension
				if (_idx != std::string::npos)
					_extension = _metaf.substr(_idx + 1);

				//If we have a path instead
				else if (std::filesystem::is_directory(_dirPath)) {
					files.push_back({ _dirPath.filename().string().c_str(), _dirPath,
						File::Filetype::eFolder, nullptr });
					continue;
				}
				else
					continue;

				//If the extension is meta
				if (!strcmp(_extension.c_str(), s_common_strs[eMetaS])) {
					nlohmann::json _j;
					std::ifstream _fp(_dirPath.string());

					//If we could read the file
					if (_fp.is_open()) {
						_fp >> _j;
						_fp.close();

						std::string&& _filename = _j[s_common_strs[ePathS]];
						size_t _idx = _filename.find_last_of('.');

						//If we could get an extension
						if (_idx != std::string::npos) {
							_extension = _filename.substr(_idx + 1);

							//If the pointed file is a level
							if (!strcmp(_extension.c_str(), "blevel"))
								files.push_back({ _j[s_common_strs[eNameS]], _dirPath,
									File::Filetype::eLevel, nullptr });

							//If the pointer file is a png
							else if (!strcmp(_extension.c_str(), "png"))
								files.push_back({ _j[s_common_strs[eNameS]], _dirPath,
									File::Filetype::eTexture,
									ResourceMgr->Load<BaleaEngine::Graphics::Texture>
									(_filename.c_str())->get() });

							//If the pointed file is a wav file
							else if (!strcmp(_extension.c_str(), "wav") ||
								!strcmp(_extension.c_str(), "mp3"))
								files.push_back({ _j[s_common_strs[eNameS]], _dirPath,
									File::Filetype::eAudio, nullptr });

							//If the pointed file is a space file
							else if (!strcmp(_extension.c_str(), "aspace"))
								files.push_back({ _j[s_common_strs[eNameS]], _dirPath,
									File::Filetype::eSpace, nullptr });

							//If the pointed file is a space file
							else if (!strcmp(_extension.c_str(), "atlas"))
								files.push_back({ _j[s_common_strs[eNameS]], _dirPath,
									File::Filetype::eAnimation, nullptr });

							//If the pointed file is none of the above
							else
								files.push_back({ _j[s_common_strs[eNameS]], _dirPath,
									File::Filetype::eUnknown, nullptr });
						}

						continue;
					}

					//If the file is source
				}
				else if (!strcmp(_extension.c_str(), "h") ||
					!strcmp(_extension.c_str(), "cpp"))
					files.push_back({ _metaf, _dirPath, File::Filetype::eSource, nullptr });
			}
		}
	}

	// ------------------------------------------------------------------------
	/*! Vector File Items Getter
	*
	*   Gets the file items and stores them in a vector
	*/ //--------------------------------------------------------------------
	static bool vector_file_items_getter(void* data, int idx, const char** out_text) noexcept {
		*out_text = reinterpret_cast<stl::Array<File>*>(data)->at(idx).mAlias.data();

		return true;
	}

	// ------------------------------------------------------------------------
	/*! Custom Constructor
	*
	*   Constructs a File Browser
	*/ //--------------------------------------------------------------------
	File_browser::File_browser(const char* title, const char* path) noexcept :
		mTitle(title),
		mSelection(0),
		mCurrentPath(path),
		mCurrentPathIsDir(true) {
		get_files_in_path(mCurrentPath, mFilesInScope);
		const char** _j = s_common_strs + eFolderS;
		const std::string _enginepath = "";

		//Load all the required Asset browser files
		for (unsigned char i = eFolderT;
			i < sizeof(s_Textures) / sizeof(BaleaEngine::Graphics::Texture*); i++, _j++) {
			s_Textures[i] = ResourceMgr->Load<BaleaEngine::Graphics::Texture>(*_j)->get();
		}
	}

	// ------------------------------------------------------------------------
	/*! Thumbnail
	*
	*   Draws a Thumbnail on the ImGui Layer
	*/ //--------------------------------------------------------------------
	bool ThumbNail(ImTextureID user_texture_id, const ImVec2& size, const char* label) noexcept {
		ImGuiWindow* _window = ImGui::GetCurrentWindow();

		//If the window is collapsed
		if (_window->SkipItems)
			return false;

		const ImGuiID _id = _window->GetID(label);
		const ImVec2 _label_size = ImGui::CalcTextSize(label, NULL, true);
		const ImVec2 _button_size = { size.x, size.y + _label_size.y * 2 };
		ImRect _bb(_window->DC.CursorPos, { _window->DC.CursorPos.x + _button_size.x + 30,
			_window->DC.CursorPos.y + _button_size.y + 30 });
		const ImRect _image_bb({ _window->DC.CursorPos.x + 15, _window->DC.CursorPos.y + 15 },
			{ _window->DC.CursorPos.x + 15 + size.x, _window->DC.CursorPos.y + 15 + size.y });
		ImGui::ItemSize(_bb);

		//If we have part of the item clipped
		if (!ImGui::ItemAdd(_bb, _id))
			return false;

		bool _hovered, _held, pressed = ImGui::ButtonBehavior(_bb, _id, &_hovered, &_held,
			ImGuiButtonFlags_PressedOnDoubleClick);
		const ImU32 _col = ImGui::GetColorU32((_held && _hovered) ? ImGuiCol_ButtonActive :
			_hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
		ImGui::RenderNavHighlight(_bb, _id);
		ImGui::RenderFrame(_bb.Min, _bb.Max, _col, true, ImClamp(15.f, 0.f, (*GImGui).Style.FrameRounding));
		_window->DrawList->AddImage(user_texture_id, _image_bb.Min, _image_bb.Max, { 0, 0 },
			{ 1, -1 });
		_bb.Min.y = _image_bb.Max.y + 15;
		ImGui::RenderTextClipped({ _bb.Min.x + (*GImGui).Style.FramePadding.x,
			_bb.Min.y + (*GImGui).Style.FramePadding.y }, { _bb.Max.x - (*GImGui).Style.FramePadding.x,
			_bb.Max.y - (*GImGui).Style.FramePadding.y }, label, NULL, &_label_size,
			(*GImGui).Style.ButtonTextAlign, &_bb);

		return pressed;
	}

	// ------------------------------------------------------------------------
	/*! Get Handle From File
	*
	*   Gets an specific file handle to represent each file type
	*/ //--------------------------------------------------------------------
	unsigned File::GetHandleFromFile() const noexcept {
		//Switch by the file type
		switch (mFiletype) {
			//case folder
		case File::Filetype::eFolder:
			return s_Textures[eFolderT]->GetGLHandle();

			//case Texture
		case File::Filetype::eTexture:
			return mCustomThumb->GetGLHandle();

			//case Source
		case File::Filetype::eSource:
			return s_Textures[eSourceT]->GetGLHandle();

			//case Audio
		case File::Filetype::eAudio:
			return s_Textures[eAudioT]->GetGLHandle();

			//case Level
		case File::Filetype::eLevel:
			return s_Textures[eLevelT]->GetGLHandle();

			//case Space
		case File::Filetype::eSpace:
			return s_Textures[eSpaceT]->GetGLHandle();

			//case Space
		case File::Filetype::eAnimation:
			return s_Textures[eAnimationT]->GetGLHandle();

			//case unknown
		default:
			return s_Textures[eUnknownT]->GetGLHandle();
		}
	}

	// ------------------------------------------------------------------------
	/*! Present
	*
	*   Presents a File Browser and returns wether we select a file or not
	*/ //--------------------------------------------------------------------
	const bool File_browser::Present(std::string& outPath) noexcept {
		//If we could begin a file explorer
		if (ImGui::Begin(mTitle)) {
			if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootWindow) && ImGui::GetIO().MouseClicked[1]) {
				ImGui::OpenPopup("File Explorer options");
			}

			if (ImGui::BeginPopup("File Explorer options")) {
				if (ImGui::BeginMenu("Add")) {
					if (ImGui::MenuItem("New Widget...")) {
					}

					ImGui::EndMenu();
				}

				static std::string foldername_;

				ImGui::InputText("", &foldername_);

				if (ImGui::MenuItem("Create Folder"))
					system(std::string("mkdir " + foldername_).c_str());

				ImGui::EndPopup();
			}

			mFilesPerRow = static_cast<int>(ImGui::GetWindowWidth() / (64 * 2));
			unsigned char _n = 0;

			//Iterate through all the files on the scope
			for (size_t i = 0; i < mFilesInScope.size(); i++) {
				auto x = mFilesInScope[i];

				//If we click on any of the thumbnails
				if (ThumbNail(reinterpret_cast<void*>(static_cast<long long>(
					x.GetHandleFromFile())),
					{ 64, 64 }, x.mAlias.c_str())) {
					mCurrentPath = x.mPath;
					mCurrentPathIsDir = std::filesystem::is_directory(mCurrentPath);

					if (mCurrentPathIsDir)
						get_files_in_path(mCurrentPath, mFilesInScope);
					else {
						outPath = x.mPath.string();
						ImGui::End();

						return true;
					}
				}

				//If there is room for more files on this line
				if (_n <= mFilesPerRow) {
					ImGui::SameLine();
					_n++;
				}
				else
					_n = 0;
			}

			ImGui::End();
		}

		return false;
	}

	// ------------------------------------------------------------------------
	/*! Custom Constructor
	*
	*   Constructs a Model given a title
	*/ //--------------------------------------------------------------------
	File_browser_modal::File_browser_modal(const char* title) noexcept :
		mTitle(title),
		mOldVisibility(false),
		mSelection(0),
		mCurrentPath(std::filesystem::current_path()),
		mCurrentPathIsDir(true) {
		get_files_in_path(mCurrentPath, mFilesInScope);
	}

	// ------------------------------------------------------------------------
	/*! Render
	*
	*   Render a browser Modal
	*/ //--------------------------------------------------------------------
	const bool File_browser_modal::Present(const bool isVisible, std::string& outPath) noexcept {
		bool _result = false;
		static char _filestring[_MAX_PATH] = { 0 };

		if (mOldVisibility != isVisible) {
			mOldVisibility = isVisible;

			//If it's visible
			if (isVisible) {
				mCurrentPath = std::filesystem::current_path();
				mCurrentPathIsDir = true;
				ImGui::OpenPopup(mTitle);
			}
		}

		static bool _isOpen = true;

		//If we can open our modal
		if (ImGui::BeginPopupModal(mTitle, &_isOpen, ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_AlwaysAutoResize)) {
			//Show the List of files
			if (ImGui::ListBox("##", &mSelection, vector_file_items_getter, &mFilesInScope,
				static_cast<int>(mFilesInScope.size()), 10)) {
				mCurrentPath = mFilesInScope[mSelection].mPath;
				mCurrentPathIsDir = std::filesystem::is_directory(mCurrentPath);

				//If the selection is a directory, repopulate the list with the contents of that directory.
				if (mCurrentPathIsDir)
					get_files_in_path(mCurrentPath, mFilesInScope);
			}

			char _buff[_MAX_PATH] = { 0 };

			ImGui::PushItemWidth(-1);
			ImGui::InputText(_buff, _buff, _MAX_PATH);

			//If we have a valid string
			if (*_buff != '\0')
				strcpy_s(_filestring, _buff);

			ImGui::PopItemWidth();

			ImGui::Spacing();
			ImGui::SameLine(ImGui::GetWindowWidth() - 60);

			// Make the "Select" button look / act disabled if the current selection is a directory.
			if (*_filestring == '\0') {
				static const ImVec4 _disabledColor = { 0.3f, 0.3f, 0.3f, 1.0f };

				ImGui::PushStyleColor(ImGuiCol_Button, _disabledColor);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, _disabledColor);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, _disabledColor);

				ImGui::Button("Save");

				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
			}
			else {
				//If we pressed Save
				if (ImGui::Button("Save")) {
					ImGui::CloseCurrentPopup();

					//If we have a Directory
					if (mCurrentPathIsDir)
						outPath = outPath + std::string("\\") + std::string(_filestring) +
						std::string(".blevel");
					else
						outPath = mCurrentPath.string();

					_result = true;
				}
			}

			ImGui::EndPopup();
		}

		return _result;
	}

	// ------------------------------------------------------------------------
	/*! Image Button With Text
	*
	*   Render a button with text
	*/ //--------------------------------------------------------------------
	static bool ImageButtonWithText(ImTextureID texId, const char* label) noexcept {
		ImGuiWindow* _window = ImGui::GetCurrentWindow();

		//If the window is not clipped
		if (_window->SkipItems)
			return false;

		const float _size = 32.f * _window->FontWindowScale * ImGui::GetIO().FontGlobalScale;
		const ImGuiID _id = _window->GetID(label);
		const ImVec2 _textSize = ImGui::CalcTextSize(label, NULL, true);
		const float _innerSpacing = 10;
		const ImRect _bb(_window->DC.CursorPos,
			{ _window->DC.CursorPos.x + _size + _innerSpacing + _textSize.x + 20,
			  _window->DC.CursorPos.y + ImMax(_size, _textSize.y) + 20 });
		ImVec2 _start(0, 0);
		_start = { _window->DC.CursorPos.x + 10, _window->DC.CursorPos.y + 10 };
		if (_size < _textSize.y)
			_start.y += (_textSize.y - _size) * .5f;
		const ImRect _image_bb(_start, { _start.x + _size, _start.y + _size });
		_start = { _window->DC.CursorPos.x + 10, _window->DC.CursorPos.y + 10 };
		_start.x += _size + _innerSpacing;
		if (_size > _textSize.y)
			_start.y += (_size - _textSize.y) * .5f;
		ImGui::ItemSize(_bb);

		//if the item is partially clipped
		if (!ImGui::ItemAdd(_bb, _id))
			return false;

		bool _hovered = false, held = false;
		bool _pressed = ImGui::ButtonBehavior(_bb, _id, &_hovered, &held);

		// Render
		const ImU32 _col = ImGui::GetColorU32((_hovered && held) ? ImGuiCol_ButtonActive : _hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
		ImGui::RenderFrame(_bb.Min, _bb.Max, _col, true, ImClamp(10.f, 0.0f, (*GImGui).Style.FrameRounding));

		_window->DrawList->AddImage(texId, _image_bb.Min, _image_bb.Max);

		//If we have a valid text size
		if (_textSize.x > 0)
			ImGui::RenderText(_start, label);

		return _pressed;
	}

	static stl::Array<File> tempfiles;

	// ------------------------------------------------------------------------
	/*! Present
	*
	*   Presents a file browser context
	*/ //--------------------------------------------------------------------
	bool File_browser_context::Present(std::string& outstr, const char* ext, const char* title) noexcept {
		//If we can begin our combo
		if (ImGui::BeginCombo(title, outstr.c_str())) {
			ImGui::PushItemWidth(256);

			get_files_in_path_recursive(std::filesystem::current_path(), tempfiles);

			//Iterate through all the files on the scope
			for (const auto& x : tempfiles) {
				std::string _filename = x.mPath.string();
				std::string _strname = x.mAlias;

				std::fstream _fp(_filename);

				if (_fp.good()) {
					nlohmann::json j_;
					_fp >> j_;
					_filename = j_[s_common_strs[ePathS]].get<std::string>();

					size_t _idx = _filename.find_last_of('.');
					std::string _extension = _filename.substr(_idx + 1);

					//if we have a match
					if (!strcmp(_extension.c_str(), ext))

						//if we click on the button
						if (ImageButtonWithText(
							reinterpret_cast<void*>(static_cast<long long>(
								x.GetHandleFromFile())), _strname.c_str())) {
							outstr = _filename;
							ImGui::PopItemWidth();
							ImGui::EndCombo();
							return true;
						}
				}
			}

			ImGui::PopItemWidth();
			ImGui::EndCombo();
		}

		return false;
	}
}