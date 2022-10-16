//
//	Console.h
//	Balea Engine
//
//	Created by Diego Revilla 11/09/20
//	Copyright © 2021 . All Rights reserved
//

#ifndef _CONSOLE__H_
#define _CONSOLE__H_

namespace BaleaEditor {
	namespace Console {
		struct Command {
		public:
			virtual void Process(const char* args) = 0;
			const char* command = nullptr;
		};

		class ConsoleD {
		public:
			void AddCommand(Command* cmd);
			void Log(const char* format, ...);
			static ConsoleD& GetInstance();

		protected:
			ConsoleD();
			ConsoleD(const ConsoleD&) = delete;
			ConsoleD(ConsoleD&&) = delete;
		};
	}
}

#define AConsole (&BaleaEditor::Console::ConsoleD::GetInstance())

#endif _CONSOLE__H_