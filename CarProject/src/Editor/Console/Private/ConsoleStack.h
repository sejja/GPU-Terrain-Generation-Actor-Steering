//
//	ConsoleStack.h
//	Balea Engine
//
//	Created by Diego Revilla on 14/09/20
//	Copyright © 2021 . All Rights reserved
//

#ifndef _CONSOLE_STACK__H_
#define _CONSOLE_STACK__H_

namespace BaleaEditor {
	namespace Console {
		namespace _private_ {
			extern stl::Array<std::string>_output_backlog_strs_;
			extern stl::Array<Command*> _commands_;
		}
	}
}

#endif