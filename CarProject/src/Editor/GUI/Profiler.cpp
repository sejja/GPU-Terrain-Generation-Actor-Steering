//
//	Profiler.cpp
//	Balea Engine
//
//	Created by Diego Revilla on 10/2/21
//	Copyright © 2021 . All Rights reserved
//

#include <Shared.h>

namespace BaleaEditor {
	Profiler& Profiler::GetInstance() {
		static Profiler prof;

		return prof;
	}
}