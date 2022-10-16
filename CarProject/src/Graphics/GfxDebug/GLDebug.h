// ----------------------------------------------------------------------------
//	Copyright (C) Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior
//	written consent of  Institute of Technology is prohibited.
//
//	File Name:		GLDebug.h
//	Purpose:		This file contains functions used to debug graphics
//	Project:		cs300_alejandro.balea_3
//	Author:			Alejandro Balea Moreno - alejandro.balea - 540002118
// ----------------------------------------------------------------------------
#pragma once

namespace Debug
{
	// Usage:
	//    Debug::CheckGLError(__LINE__, __FILE__);
	void CheckGLError(int _LINE_, const char* _FILE_);

}

#define gl_check_error()	Debug::CheckGLError(__LINE__, __FILE__)
