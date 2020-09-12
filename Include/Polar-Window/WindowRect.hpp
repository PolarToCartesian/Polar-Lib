#ifndef __POLAR__FILE_WINDOW_RECT_HPP
#define __POLAR__FILE_WINDOW_RECT_HPP

#include <Polar-Build/Polar-Build.hpp>

#ifdef __POLAR__TARGET_WINDOWS
    #include <Windows.h>
#endif // __POLAR__TARGET_WINDOWS

#include <cstdint>

namespace PL {

    struct Rect {
		std::int32_t left   = 0u;
		std::int32_t top    = 0u;
		std::int32_t right  = 0u;
		std::int32_t bottom = 0u;

#ifdef __POLAR__TARGET_WINDOWS

        inline Rect(const RECT& rect) noexcept : left(rect.left), top(rect.top), right(rect.right), bottom(rect.bottom) {  }

		inline operator const RECT&() const noexcept { return RECT{ left, top, right, bottom }; }

#endif // __POLAR__TARGET_WINDOWS
	}; // struct Rect

}; // namespace PL

#endif // __POLAR__FILE_WINDOW_RECT_HPP