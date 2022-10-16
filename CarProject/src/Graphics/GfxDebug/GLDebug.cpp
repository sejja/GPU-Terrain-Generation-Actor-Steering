// ----------------------------------------------------------------------------
//	Copyright (C) Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior
//	written consent of  Institute of Technology is prohibited.
//
//	File Name:		GLDebug.cpp
//	Purpose:		This file contains functions used to debug graphics
//	Project:		cs300_alejandro.balea_3
//	Author:			Alejandro Balea Moreno - alejandro.balea - 540002118
// ----------------------------------------------------------------------------
#include <Shared.h>

namespace Debug
{
	void CheckGLError(int _LINE_, const char* _FILE_)
	{
		std::cout << "Gl Error check at line " << _LINE_ << " of file " << _FILE_ << ": ";
		switch (glGetError())
		{
		case GL_NO_ERROR:
			std::cout << "	 - No errors found." << std::endl; break;
		case GL_INVALID_ENUM:
			std::cout << "	 - ERROR: Invalid value specified for an enumerated argument." << std::endl; break;
		case GL_INVALID_VALUE:
			std::cout << "	 - ERROR: Numeric argument out of range." << std::endl; break;
		case GL_INVALID_OPERATION:
			std::cout << "	 - ERROR: Specified operation not allowed in the current state." << std::endl; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			std::cout << "	 - ERROR: The framebuffer object is not complete." << std::endl; break;
		case GL_OUT_OF_MEMORY:
			std::cout << "	 - ERROR: Not enough memory left to execute the command." << std::endl; break;
		case GL_STACK_UNDERFLOW:
			std::cout << "	 - ERROR: An attempt has been made to perform an operation that would cause an internal stack to underflow." << std::endl; break;
		case GL_STACK_OVERFLOW:
			std::cout << "   - ERROR: An attempt has been made to perform an operation that would cause an internal stack to overflow." << std::endl; break;
		}
	}
}