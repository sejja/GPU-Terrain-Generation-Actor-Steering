//
//	Singleton.h
//	Balea Engine
//
//	Created by Alejandro Balea 06/05/21
//	Copyright © 2021 . All Rights reserved
//

#ifndef _SINGLETON__H_
#define _SINGLETON__H_

#define Singleton(class_name)\
protected:\
	class_name() noexcept {}\
   ~class_name() noexcept {}\
public:\
	static class_name& Instance() noexcept {\
		static class_name mpInstance;\
		return mpInstance;\
	}
#endif