//
//	MathSerializer.cpp
//	Balea Engine
//
//	Created by Diego Revilla on 12/05/21
//	Copyright © 2021 . All Rights reserved
//

#include <Shared.h>

namespace BaleaEngine {
	namespace Serializers {
		namespace {
			constexpr const char* s_common_str[] = { "x", "y", "z" };
			constexpr const size_t s_x_idx = 0, s_y_idx = 1, s_z_idx = 2;
		}

		// ------------------------------------------------------------------------
		/*! Vector3D To Json
		*
		*   Returns a Json from a Vector3D
		*/ // ---------------------------------------------------------------------
		nlohmann::json MathSerializer::Vector3DToJson(const Math::Vector3D& vec) noexcept {
			nlohmann::json _val;
			_val[s_common_str[s_x_idx]] = vec.x, _val[s_common_str[s_y_idx]] = vec.y,
				_val[s_common_str[s_z_idx]] = vec.z;
			return _val;
		}

		// ------------------------------------------------------------------------
		/*! Vector3D From Json
		*
		*   Returns a Vector3D from a Json
		*/ // ---------------------------------------------------------------------
		Math::Vector3D MathSerializer::Vector3DFromJson(const nlohmann::json& val) noexcept {
			Math::Vector3D _vec;

			_vec.x = val[s_common_str[s_x_idx]].get<float>(), _vec.y = val[s_common_str[s_y_idx]].get<float>(),
				_vec.z = val[s_common_str[s_z_idx]].get<float>();
			return _vec;
		}

		// ------------------------------------------------------------------------
		/*! Vector2D To Json
		*
		*   Returns a Json from a Vector2D
		*/ // ---------------------------------------------------------------------
		nlohmann::json MathSerializer::Vector2DToJson(const Math::Vector2D& vec) noexcept {
			nlohmann::json _val;

			_val[s_common_str[s_x_idx]] = vec.x, _val[s_common_str[s_y_idx]] = vec.y;
			return _val;
		}

		// ------------------------------------------------------------------------
		/*! Vector2D From Json
		*
		*   Returns a Vector2D from a Json
		*/ // ---------------------------------------------------------------------
		Math::Vector2D MathSerializer::Vector2DFromJson(const nlohmann::json& val) noexcept {
			Math::Vector2D _vec;

			_vec.x = val[s_common_str[s_x_idx]].get<float>(), _vec.y = val[s_common_str[s_y_idx]].get<float>();
			return _vec;
		}
	}
}