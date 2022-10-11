//
//	Header.h
//	Balea Engine
//
//	Created by Diego Revilla on 23/09/20
//	Copyright © 2021 Digipen. All Rights reserved
//

#ifndef _HEADER__H_
#define _HEADER__H_

namespace BaleaEditor {
	struct HeaderState : public BaleaEditor::IEditorState {
#pragma region // Functions
		void Load() noexcept override;
		void PreDraw() noexcept override;
		void Present() noexcept override;
#pragma endregion
	};
}

#endif
