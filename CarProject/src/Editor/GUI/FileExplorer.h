//
//	FileExplorer.h
//	Balea Engine
//
//	Created by Diego Revilla on 31/10/20
//	Copyright © 2021 Digipen. All Rights reserved
//

#ifndef _FILE_EXPLORER__H_
#define _FILE_EXPLORER__H_

namespace BaleaEditor {
	struct File {
#pragma region // Declarations
		enum class Filetype : unsigned char {
			eFolder,
			eLevel,
			eTexture,
			eSource,
			eAudio,
			eSpace,
			eAnimation,
			eUnknown
		};
#pragma endregion

#pragma region // Functions
		unsigned GetHandleFromFile() const noexcept;
#pragma endregion

#pragma region // Members
		std::string mAlias;
		std::filesystem::path mPath;
		Filetype mFiletype;
		BaleaEngine::Graphics::Texture* mCustomThumb = nullptr;
#pragma endregion
	};

	class File_browser {
#pragma region // Functions
	public:
		explicit File_browser(const char* title, const char* path = nullptr) noexcept;
		const bool Present(std::string& outPath) noexcept;
#pragma endregion

#pragma region // Members
	private:
		const char* mTitle;
		int mSelection;
		std::filesystem::path mCurrentPath;
		bool mCurrentPathIsDir;
		stl::Array<File> mFilesInScope;
		unsigned char mFilesPerRow = 9;
#pragma endregion
	};

	class File_browser_modal {
#pragma region // Functions
	public:
		explicit File_browser_modal(const char* title) noexcept;
		const bool Present(const bool isVisible, std::string& outPath) noexcept;
#pragma endregion

#pragma region // Members
	private:
		const char* mTitle;
		bool mOldVisibility;
		int mSelection;
		std::filesystem::path mCurrentPath;
		bool mCurrentPathIsDir;
		stl::Array<File> mFilesInScope;
#pragma endregion
	};

	class File_browser_context {
#pragma region // Functions
	public:
		bool Present(std::string& outstr, const char* ext, const char* title) noexcept;
#pragma endregion
	};
}

#endif
