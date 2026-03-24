#pragma once

#ifdef NX_PLATFORM_WINDOWS
	#ifdef NX_BUILD_DLL
		#define NX_API __declspec(dllexport)
	#else
		#define NX_API __declspec(dllimport)
	#endif
#else
	#error Nexus only supports Windows
#endif

#define BIT(x) (1 << x)

