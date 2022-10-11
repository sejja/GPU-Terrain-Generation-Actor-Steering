//
//	PropertiesGUI.h
//	Balea Engine
//
//	Created by Diego Revilla on 3/2/20
//	Copyright © 2021 Digipen. All Rights reserved
//

#ifndef _PROPERTIES_GUI__H_
#define _PROPERTIES_GUI__H_

namespace BaleaEditor {
	class PropertiesWindow : public IWindow {
	public:
		void Present() noexcept override;

	private:
		bool mSaving = false;
	};

	class SceneWindow : public IWindow {
	public:
		void Present() noexcept override;
	};

	class PaletteWindow : public IWindow {
	public:
		void Present() noexcept override;

	private:
		BaleaEngine::ICreator* mCreator = nullptr;
	};
}

#endif