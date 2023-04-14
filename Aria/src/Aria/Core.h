#pragma once

#ifdef ARIA_PLATFORM_WINDOWS
	#ifdef ARIA_BUILD_DLL
		#define ARIA_API __declspec(dllexport)
	#else
		#define ARIA_API __declspec(dllimport)
	#endif
#else
	#error Aria only supports Windows!
#endif