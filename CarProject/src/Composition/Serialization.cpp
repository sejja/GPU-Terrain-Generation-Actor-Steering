//
//	Serialization.cpp
//	Balea Engine
//
//	Created by Alejandro Balea on 06/05/21
//	Copyright © 2021 Digipen. All Rights reserved
//

#include <Shared.h>

#define SERIAL_	BaleaEngine::Serializable::

namespace BaleaEngine {
	// ------------------------------------------------------------------------
	/*! Operator <<
	*
	*   Wrapper operator
	*/ //--------------------------------------------------------------------
	SERIAL_ json& Serializable::operator<<(json& val) const {
		ToJson(val);
		return val;
	}

	// ------------------------------------------------------------------------
	/*! Operator >>
	*
	*   Wrapper operator
	*/ //--------------------------------------------------------------------
	const SERIAL_ json& Serializable::operator>>(const json& val) {
		FromJson(val);
		return val;
	}
}