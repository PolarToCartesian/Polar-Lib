#ifndef __POLAR__FILE_SHAPES_HPP
#define __POLAR__FILE_SHAPES_HPP

#include <cstdint>

namespace PL {

	struct Rect {
		std::uint16_t left   = 0u;
		std::uint16_t top    = 0u;
		std::uint16_t right  = 0u;
		std::uint16_t bottom = 0u;
	}; // struct Rect

}; // namespace PL

#endif // __POLAR__FILE_SHAPES_HPP