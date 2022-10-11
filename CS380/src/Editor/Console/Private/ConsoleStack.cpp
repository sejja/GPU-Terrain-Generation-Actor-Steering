//
//	ConsoleStack.cpp
//	Balea Engine
//
//	Created by Diego Revilla on 14/09/20
//	Copyright © 2021 Digipen. All Rights reserved
//

#include <Shared.h>

namespace BaleaEditor {
	namespace Console {
		namespace _private_ {
			stl::Array<std::string>_output_backlog_strs_;
			stl::Array<Command*> _commands_;
		}
	}
}