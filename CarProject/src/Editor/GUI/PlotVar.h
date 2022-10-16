//
//	PlotVar.h
//	Balea Engine
//
//	Created by Diego Revilla on 10/3/20
//	Copyright © 2021 . All Rights reserved
//

#ifndef _PLOTVAR__H_
#define _PLOTVAR__H_

namespace BaleaEditor {
	STATIC PlotVar(const char* label, const float value, const float scale_min, const float scale_max, int
		buffer_size = 0) noexcept;
}

#endif
