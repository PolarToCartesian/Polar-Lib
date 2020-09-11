#ifndef __POLAR__FILE_COLOR_HPP
#define __POLAR__FILE_COLOR_HPP

#include <cstdint>

namespace PL {

    struct Coloru8 {
		union {
			struct {
				std::uint8_t r;
				std::uint8_t g;
				std::uint8_t b;
				std::uint8_t a;
			};

			struct {
				std::uint8_t red;
				std::uint8_t green;
				std::uint8_t blue;
				std::uint8_t alpha;
			};

			std::uint32_t color32;
		};
	}; // struct Coloru8

	constexpr const Coloru8 COLOR_U8_RED   = Coloru8{ 255u, 0u,   0u,   255u };
	constexpr const Coloru8 COLOR_U8_GREEN = Coloru8{ 0u,   255u, 0u,   255u };
	constexpr const Coloru8 COLOR_U8_BLUE  = Coloru8{ 0u,   0u,   255u, 255u };
	constexpr const Coloru8 COLOR_U8_WHITE = Coloru8{ 255u, 255u, 255u, 255u };
	constexpr const Coloru8 COLOR_U8_BLACK = Coloru8{ 0u,   0u,   0u,   255u };

    struct Colorf32 {
		union {
			struct {
				float r;
				float g;
				float b;
				float a;
			};

			struct {
				float red;
				float green;
				float blue;
				float alpha;
			};
		};
	}; // struct Colorf32

	constexpr const Colorf32 COLOR_F32_RED   = Colorf32{1.f, 0.f, 0.f, 1.f};
	constexpr const Colorf32 COLOR_F32_GREEN = Colorf32{0.f, 1.f, 0.f, 1.f};
	constexpr const Colorf32 COLOR_F32_BLUE  = Colorf32{0.f, 0.f, 1.f, 1.f};
	constexpr const Colorf32 COLOR_F32_WHITE = Colorf32{1.f, 1.f, 1.f, 1.f};
	constexpr const Colorf32 COLOR_F32_BLACK = Colorf32{0.f, 0.f, 0.f, 1.f};

}; // namespace PL

#endif // __POLAR__FILE_COLOR_HPP