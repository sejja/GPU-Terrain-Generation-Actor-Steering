//
//	Console.cpp
//	Balea Engine
//
//	Created by Diego Revilla 11/09/20
//	Copyright © 2021 . All Rights reserved
//

#include <Shared.h>

namespace BaleaEditor {
	namespace Console {
		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*   Constructs a console
		*/ //--------------------------------------------------------------------
		ConsoleD::ConsoleD() {}

		// ------------------------------------------------------------------------
		/*! Get Instance
		*
		*   Returns an instance of the console
		*/ //--------------------------------------------------------------------
		ConsoleD& ConsoleD::GetInstance() {
			static ConsoleD consol_;

			return consol_;
		}

		// ------------------------------------------------------------------------
		/*! Add Command
		*
		*   Adds a Command to the console
		*/ //--------------------------------------------------------------------
		void ConsoleD::AddCommand(Command* cmd) {
			_private_::_commands_.push_back(cmd);
		}

		// ------------------------------------------------------------------------
		/*! Log
		*
		*   Logs something onto screen
		*/ //--------------------------------------------------------------------
		void ConsoleD::Log(const char* fmt, ...) {
			va_list _ap;
			std::string _temp;

			va_start(_ap, fmt);

			//For every character, determine what information to print
			for (char* p = const_cast<char*>(fmt); *p; p++) {
				//If it's a regular character
				if (*p != '%') {
					_temp.push_back(*p);

					continue;
				}

				//else, branch by data type
				switch (*++p) {
					//Integer
				case 'i':
					_temp += std::to_string(va_arg(_ap, int));

					break;

					//Float
				case 'f':
					_temp += std::to_string(va_arg(_ap, float));

					break;

					//String
				case 's':
					_temp += va_arg(_ap, char*);

					break;

					//Boolean
				case 'b':
					_temp += va_arg(_ap, bool) ? "true" : "false";

					break;

					//No case, just return the regular character
				default:
					_temp.push_back(*p);
				}
			}

			va_end(_ap);
			_private_::_output_backlog_strs_.push_back(_temp);
		}
	}
}