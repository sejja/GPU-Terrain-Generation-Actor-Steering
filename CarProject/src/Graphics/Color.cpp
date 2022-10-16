//
//	Color.cpp
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 . All Rights reserved
//

#include <Shared.h>

namespace BaleaEngine {
	namespace Graphics {
		Color::Color(const float _r, const float _g, const float _b, const float _a) noexcept :
			r(_r), g(_g), b(_b), a(_a)
		{
		}
		Color::Color(const glm::vec4& col) noexcept : r(col.x), g(col.y), b(col.z), a(col.w)
		{
		}
	}
}