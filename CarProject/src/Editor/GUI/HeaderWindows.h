//
//	HeaderWindows.h
//	Balea Engine
//
//	Created by Diego Revilla on 23/09/20
//	Copyright © 2021 . All Rights reserved
//

#ifndef _HEADER_WINDOWS__H_
#define _HEADER_WINDOWS__H_

namespace BaleaEditor {
	class ConsoleWindow : public BaleaEditor::IWindow {
#pragma region // Constructors & Destructors
	public:
		ConsoleWindow() noexcept;
		~ConsoleWindow() noexcept;
#pragma endregion

#pragma region // Functions
		void AddLog(const char* str) noexcept;
		void Present() noexcept override;
	private:
		static int TextEditCallbackStub(ImGuiInputTextCallbackData* data) noexcept;
		void ClearLog() noexcept;
#pragma endregion

#pragma region // Members
		char mInputBuf[256];
		ImVector<char*> mItems;
		bool mScrollToBottom = true;
#pragma endregion
	};

	class FPSProfiler : public BaleaEditor::IWindow {
#pragma region // Functions
		void Present() noexcept override;
#pragma endregion
	};

	class ExplorerWindow : public BaleaEditor::IWindow {
#pragma region // Constructors & Destructors
	public:
		ExplorerWindow() noexcept;
#pragma endregion

#pragma region // Functions
		void Present() noexcept override;
		std::string GetPath() noexcept;
		std::string&& GetCurrentFile() noexcept;
		void SetPath(const std::string& file) noexcept;
		void SetCurrentFile(const std::string& file) noexcept;
#pragma endregion

#pragma region //Members
	private:
		std::string mPath;
		std::string mCurrentfile;
		File_browser mFileBrowser;
#pragma endregion
	};

	class ImporterWindowModal : public BaleaEditor::IWindow {
#pragma region // Functions
	public:
		void Present() noexcept override;
#pragma endregion

#pragma region // Members
	private:
		BaleaEditor::File_browser_modal fileBrowser = BaleaEditor::File_browser_modal("Save to");
#pragma endregion
	};

	class ResourceWindow : public BaleaEditor::IWindow {
#pragma region // Functions
	public:
		void Present() noexcept override;
#pragma endregion
	};
}
#endif