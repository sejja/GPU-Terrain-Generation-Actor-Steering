//
//	DrawingFunctions.h
//	Balea Engine
//
//	Created by Diego Revilla on 27/10/20
//	Copyright © 2021 . All Rights reserved
//

#ifndef _DRAWING_FUNCTIONS__H_
#define _DRAWING_FUNCTIONS__H_

namespace BaleaEditor {
	class UIPropertyDraw {
	public:
		DONTDISCARD bool operator<<(const std::pair<const char*, BaleaEngine::Math::Vector3D*>& info) noexcept;
		DONTDISCARD bool operator<<(const std::pair<const char*, int*>& info) noexcept;
		DONTDISCARD bool operator<<(const std::pair<const char*, unsigned*>& info) noexcept;
		DONTDISCARD bool operator<<(const std::tuple<const char*, int*, const char*>& info) noexcept;
		DONTDISCARD bool operator<<(const std::tuple<const char*, BaleaEngine::Graphics::Color*, size_t>& info) noexcept;

		template<typename T>
		DONTDISCARD bool inline operator<<(const std::pair<const char*, T&>& info) noexcept {}
	};
}

#endif