#ifndef __POLAR__FILE_VECTOR_HPP
#define __POLAR__FILE_VECTOR_HPP

#include <ostream>

#if !defined(__POLAR__NO_SSE)
    #include <xmmintrin.h>
#endif

#if !defined(__POLAR__NO_SSE_4_1)
    #include <smmintrin.h>
#endif

namespace PL {

    template <typename _T, size_t _N>
	struct VectorComponents {  };

	template <typename _T> struct VectorComponents<_T, 1u> { _T x; };
	template <typename _T> struct VectorComponents<_T, 2u> : VectorComponents<_T, 1u> { _T y; };
	template <typename _T> struct VectorComponents<_T, 3u> : VectorComponents<_T, 2u> { _T z; };
	template <typename _T> struct VectorComponents<_T, 4u> : VectorComponents<_T, 3u> { _T w; };

	template <typename _T> using Vec1 = VectorComponents<_T, 1u>;
	template <typename _T> using Vec2 = VectorComponents<_T, 2u>;
	template <typename _T> using Vec3 = VectorComponents<_T, 3u>;
	template <typename _T> using Vec4 = VectorComponents<_T, 4u>;

	struct Vec4f32 {
		union {
			struct {
				float x, y, z, w;
			};

#if !defined(__POLAR__NO_SSE)
			__m128 reg;
#endif
		};

#if !defined(__POLAR__NO_SSE)
		inline Vec4f32(const __m128& reg) { this->Set(reg); }
#endif

		inline Vec4f32(const float x = 0, const float y = 0, const float z = 0, const float w = 0) noexcept {
			this->Set(x, y, z, w);
		}

#if !defined(__POLAR__NO_SSE)
		inline void Set(const __m128& reg) noexcept { this->reg = reg; }
#endif

		inline void Set(const float x = 0, const float y = 0, const float z = 0, const float w = 0) noexcept {
#if !defined(__POLAR__NO_SSE)
			this->reg = _mm_set_ps(w, z, y, x);
#else
			this->x = x; this->y = y; this->z = z; this->w = w;
#endif
		}

		inline Vec4f32 operator+(const Vec4f32& o) const noexcept {
#if !defined(__POLAR__NO_SSE)
			return Vec4f32(_mm_add_ps(this->reg, o.reg));
#else
			return Vec4f32(this->x + o.x, this->y + o.y, this->z + o.z, this->w + o.w);
#endif
		}

		inline Vec4f32 operator-(const Vec4f32& o) const noexcept {
#if !defined(__POLAR__NO_SSE)
			return Vec4f32(_mm_sub_ps(this->reg, o.reg));
#else
			return Vec4f32(this->x - o.x, this->y - o.y, this->z - o.z, this->w - o.w);
#endif
		}

		inline Vec4f32 operator*(const Vec4f32& o) const noexcept {
#if !defined(__POLAR__NO_SSE)
			return Vec4f32(_mm_mul_ps(this->reg, o.reg));
#else
			return Vec4f32(this->x * o.x, this->y * o.y, this->z * o.z, this->w * o.w);
#endif
		}

		inline Vec4f32 operator/(const Vec4f32& o) const noexcept {
#if !defined(__POLAR__NO_SSE)
			return Vec4f32(_mm_div_ps(this->reg, o.reg));
#else
			return Vec4f32(this->x / o.x, this->y / o.y, this->z / o.z, this->w / o.w);
#endif
		}

		inline void operator+=(const Vec4f32& o) noexcept { *this = this->operator+(o); }
		inline void operator-=(const Vec4f32& o) noexcept { *this = this->operator-(o); }
		inline void operator*=(const Vec4f32& o) noexcept { *this = this->operator*(o); }
		inline void operator/=(const Vec4f32& o) noexcept { *this = this->operator/(o); }

		inline Vec4f32 operator+(const float& n) noexcept {
#if !defined(__POLAR__NO_SSE)
			return Vec4f32(_mm_add_ps(this->reg, _mm_set_ps1(n)));
#else
			return (*this) + Vec4f32{ n, n, n, n };
#endif
		}

		inline Vec4f32 operator-(const float& n) noexcept {
#if !defined(__POLAR__NO_SSE)
			return Vec4f32(_mm_sub_ps(this->reg, _mm_set_ps1(n)));
#else
			return (*this) - Vec4f32{ n, n, n, n };
#endif
		}

		inline Vec4f32 operator*(const float& n) noexcept {
#if !defined(__POLAR__NO_SSE)
			return Vec4f32(_mm_mul_ps(this->reg, _mm_set_ps1(n)));
#else
			return (*this) * Vec4f32{n, n, n, n};
#endif
		}

		inline Vec4f32 operator/(const float& n) noexcept {
#if !defined(__POLAR__NO_SSE)
			return Vec4f32(_mm_div_ps(this->reg, _mm_set_ps1(n)));
#else
			return (*this) / Vec4f32 { n, n, n, n };
#endif
		}

		inline Vec4f32 operator+=(const float& n) noexcept { *this = this->operator+(n); }
		inline Vec4f32 operator-=(const float& n) noexcept { *this = this->operator-(n); }
		inline Vec4f32 operator*=(const float& n) noexcept { *this = this->operator*(n); }
		inline Vec4f32 operator/=(const float& n) noexcept { *this = this->operator/(n); }

		inline float GetLength() const noexcept { return std::sqrtf(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w); }

		inline void  Normalize() noexcept { this->operator/=(this->GetLength()); }

		// --- Static Functions ---

		static inline float DotProduct(const Vec4f32& a, const Vec4f32& b) noexcept {
#if !defined(__POLAR__NO_SSE) && !defined(__POLAR__NO_SSE_4_1)
			return _mm_cvtss_f32(_mm_dp_ps(a.reg, b.reg, 0xFF));
#else
			return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
#endif
		}

		static inline Vec4f32 Normalized(const Vec4f32& v) noexcept {
			return v / v.GetLength();
		}

		static inline Vec4f32 CrossProduct(const Vec4f32& a, const Vec4f32& b) {
			const Vec4f32 tempA1(a.y, a.z, a.x, 0);
			const Vec4f32 tempB1(b.z, b.x, b.y, 0);
			const Vec4f32 tempA2(a.z, a.x, a.y, 0);
			const Vec4f32 tempB2(b.y, b.z, b.x, 0);

			return tempA1 * tempB1 - tempA2 * tempB2;
		}
	}; // struct Vec4f32

	inline Vec4f32 operator+(const float& n, const Vec4f32& v) noexcept { return v * n; }
	inline Vec4f32 operator-(const float& n, const Vec4f32& v) noexcept { return v * n; }
	inline Vec4f32 operator*(const float& n, const Vec4f32& v) noexcept { return v * n; }
	inline Vec4f32 operator/(const float& n, const Vec4f32& v) noexcept { return v * n; }

	inline std::ostream& operator<<(std::ostream& stream, const Vec4f32& v) noexcept {
		stream << '(' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ')';

		return stream;
	}

}; // namespace PL

#endif // __POLAR__FILE_VECTOR_HPP