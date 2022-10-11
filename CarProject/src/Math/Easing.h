//
//	Easing.h
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 Digipen. All Rights reserved
//

#ifndef _EASING__H_
#define _EASING__H_

namespace BaleaEngine {
	namespace Math {
		// ------------------------------------------------------------------------
		/*! Ease In Quad
		*
		*   Performs an Ease In Quad Lerping Algorythm
		*/ // ---------------------------------------------------------------------
		template <typename T>
		auto EaseInQuad(const T& min, const T& max, float normTime) noexcept -> decltype(T()* float())
		{
			return (min + (max - min) * normTime * normTime);
		}

		// ------------------------------------------------------------------------
		/*! Ease Out Quad
		*
		*   Performs an Ease Out Quad Lerping Algorythm
		*/ // ---------------------------------------------------------------------
		template <typename T>
		auto EaseOutQuad(const T& min, const T& max, float normTime) noexcept -> decltype(T()* float())
		{
			normTime--;
			return (min + (max - min) * (1.0f - normTime * normTime));
		}

		// ------------------------------------------------------------------------
		/*! Ease In Out Quad
		*
		*   Performs an Ease In/Out Quad Lerping Algorythm depeding on "normTime"
		*/ // ---------------------------------------------------------------------
		template <typename T>
		auto EaseInOutQuad(const T& min, const T& max, float normTime) noexcept -> decltype(T()* float())
		{
			if (normTime < 0.5f)
				return (min + (max - min) * (2.0f * normTime * normTime));
			normTime--;
			return (min + (max - min) * (1.0f - 2.0f * normTime * normTime));
		}
	}
}

#endif