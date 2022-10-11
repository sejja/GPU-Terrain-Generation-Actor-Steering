//
//	Saw.h
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 Digipen. All Rights reserved
//

#ifndef _SAW__H_
#define _SAW__H_

namespace BaleaEngine {
	namespace Math {
		// ------------------------------------------------------------------------
		/*! Saw
		*
		*   Performs a Saw Function interval
		*/ // ---------------------------------------------------------------------
		template<typename T>
		T Saw(const T& in, const T& max) noexcept {

#ifdef _WIN64  
			using entire_t = __int64;
#else
			using entire_t = __int32;
#endif
			const entire_t _in_int = static_cast<entire_t>(in);
			const entire_t _max_int = static_cast<entire_t>(max);

			return max - (sizeof(T) <= 4 ? stl::Absolute(in - (max + static_cast<T>(_in_int - _in_int % (_max_int << 1))))
				: stl::Absolute(in - (max + static_cast<T>(_in_int - _in_int % (_max_int << 1)))));
		}
	}
}

#endif