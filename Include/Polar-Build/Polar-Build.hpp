#ifndef __POLAR__FILE_TARGET_HPP
#define __POLAR__FILE_TARGET_HPP

#if defined(_WIN32)

    #define __POLAR__TARGET_WINDOWS
    
    #if defined(_WIN64)
        #define __POLAR__TARGET_64_BIT
    #else
        #define __POLAR__TARGET_32_BIT
    #endif

#elif defined(__ANDROID__)

    #define __POLAR__TARGET_ANDROID

#elif defined(__linux__)

    #define __POLAR__TARGET_LINUX

#elif defined(__APPLE__) && defined(__MACH__)

    #include <TargetConditionals.h>

	#if TARGET_IPHONE_SIMULATOR
		#define __POLAR__TARGET_IOS
	#elif TARGET_OS_IPHONE == 1
		#define __POLAR__TARGET_IOS
	#elif TARGET_OS_MAC == 1
		#defined __POLAR__TARGET_OSX
	#else
        #error The Polar Engine Doesn't Recognise Your Operating System
    #endif

#else
    #error The Polar Engine Doesn't Recognise Your Operating System
#endif

#if defined(_NDEBUG) || defined(NDEBUG)
	#define __POLAR__TARGET_RELEASE
#else
	#define __POLAR__TARGET_DEBUG
#endif

#endif // #ifndef __POLAR__FILE_TARGET