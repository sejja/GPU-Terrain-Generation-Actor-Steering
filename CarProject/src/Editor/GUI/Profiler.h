//
//	Profiler.h
//	Balea Engine
//
//	Created by Diego Revilla on 10/2/21
//	Copyright © 2021 . All Rights reserved
//

#ifndef _PROFILER__H_
#define _PROFILER__H_

namespace BaleaEditor {
	class Profiler {
#pragma region // Functions
	public:
		static Profiler& GetInstance();
		void inline LogToProfile(const char* str, const float val) noexcept;

#pragma endregion

#pragma region // Members
	private:
		stl::Map<const char*, float> mProfiledValues;
#pragma endregion
	};

	// ------------------------------------------------------------------------
	/*! Log To Profile
	*
	*   Saves a Certain value to a Profile stream to be shown next frame
	*/ //--------------------------------------------------------------------
	void inline Profiler::LogToProfile(const char* str, const float val) noexcept {
		mProfiledValues[str] = val;
	}
}

#endif