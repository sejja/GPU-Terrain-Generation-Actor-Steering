//
//  Common Defines
//  Astro Engine
//
//  Created by Diego Revilla on 28/09/20.
//  Copyright � 2020 Digipen. All rights reserved.
//

#ifndef _COMMON_DEFINES__H_
#define _COMMON_DEFINES__H_

#define STATIC	const void
#define DONTDISCARD [[nodiscard]]
#define VIRTUAL virtual void
#define UNLIKELY [[unlikely]]
#define LIKELY	[[likely]]
#define NOSPACE	[[no_unique_address]]
#define CATRIBUTE	__declspec(naked)
#define UNUSED(a)		(void)(a)

#pragma warning(disable:4251)

#endif