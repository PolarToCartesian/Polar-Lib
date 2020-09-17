#ifndef __POLAR__FILE_VECTOR_HPP
#define __POLAR__FILE_VECTOR_HPP

#include <cmath>
#include <math.h>
#include <ostream>
#include <algorithm>

#if !defined(__POLAR__NO_SSE)
    #include <xmmintrin.h>
#endif

#if !defined(__POLAR__NO_SSE_4_1)
    #include <smmintrin.h>
#endif

namespace PL {

    template <typename _T, size_t _N>
	struct PointComponents {  };

	template <typename _T> struct PointComponents<_T, 1u> { _T x; };
	template <typename _T> struct PointComponents<_T, 2u> : PointComponents<_T, 1u> { _T y; };
	template <typename _T> struct PointComponents<_T, 3u> : PointComponents<_T, 2u> { _T z; };
	template <typename _T> struct PointComponents<_T, 4u> : PointComponents<_T, 3u> { _T w; };

	template <typename _T>
	std::ostream& operator<<(std::ostream& stream, const PointComponents<_T, 1u>& pc) noexcept {
		stream << '(' << pc.x << ')';
		return stream;
	}

	template <typename _T>
	std::ostream& operator<<(std::ostream& stream, const PointComponents<_T, 2u>& pc) noexcept {
		stream << '(' << pc.x << ", " << pc.y << ')';
		return stream;
	}

	template <typename _T>
	std::ostream& operator<<(std::ostream& stream, const PointComponents<_T, 3u>& pc) noexcept {
		stream << '(' << pc.x << ", " << pc.y << ", " << pc.z << ')';
		return stream;
	}

	template <typename _T>
	std::ostream& operator<<(std::ostream& stream, const PointComponents<_T, 4u>& pc) noexcept {
		stream << '(' << pc.x << ", " << pc.y << ", " << pc.z << pc.w << ')';
		return stream;
	}

	template <typename _T> using Point1 = PointComponents<_T, 1u>;
	template <typename _T> using Point2 = PointComponents<_T, 2u>;
	template <typename _T> using Point3 = PointComponents<_T, 3u>;
	template <typename _T> using Point4 = PointComponents<_T, 4u>;

	struct Vec4f32 {
		union {
			struct {
				float x, y, z, w;
			};

#if !defined(__POLAR__NO_SSE)
			__m128 reg;
#endif // !__POLAR__NO_SSE
		}; // anonymous union

#if !defined(__POLAR__NO_SSE)
		inline Vec4f32(const __m128& i) { this->Set(i); }
#endif // !__POLAR__NO_SSE

		inline Vec4f32(const float x = 0.f, const float y = 0.f, const float z = 0.f, const float w = 0.f) noexcept {
			this->Set(x, y, z, w);
		}

#if !defined(__POLAR__NO_SSE)
		inline void Set(const __m128& i) noexcept {
			this->reg = i;
		}
#endif // !__POLAR__NO_SSE
		inline void Set (const float x, const float y, const float z, const float w) noexcept {
#if !defined(__POLAR__NO_SSE)
			this->reg = _mm_set_ps(w, z, y, x);
#else
			this->x = x; this->y = y;
			this->z = z; this->w = w;
#endif
		}

		inline void Set1(const float n) noexcept {
#if !defined(__POLAR__NO_SSE)
			this->reg = _mm_set_ps1(n);
#else
			this->Set(n, n, n, n);
#endif
		}

		inline Vec4f32& operator+=(const Vec4f32& o) noexcept {
#if !defined(__POLAR__NO_SSE)
			this->reg = _mm_add_ps(this->reg, o.reg);
#else
			this->x += o.x; this->y += o.y;
			this->z += o.z; this->w += o.w;
#endif

			return *this;
		}

		inline Vec4f32& operator-=(const Vec4f32& o) noexcept {
#if !defined(__POLAR__NO_SSE)
			this->reg = _mm_sub_ps(this->reg, o.reg);
#else
			this->x -= o.x; this->y -= o.y;
			this->z -= o.z; this->w -= o.w;
#endif

			return *this;
		}

		inline Vec4f32& operator*=(const Vec4f32& o) noexcept {
#if !defined(__POLAR__NO_SSE)
			this->reg = _mm_mul_ps(this->reg, o.reg);
#else
			this->x *= o.x; this->y *= o.y;
			this->z *= o.z; this->w *= o.w;
#endif

			return *this;
		}

		inline Vec4f32& operator/=(const Vec4f32& o) noexcept {
#if !defined(__POLAR__NO_SSE)
			this->reg = _mm_mul_ps(this->reg, o.reg);
#else
			this->x /= o.x; this->y /= o.y;
			this->z /= o.z; this->w /= o.w;
#endif

			return *this;
		}

		inline Vec4f32& operator+=(const float n) noexcept {
#if !defined(__POLAR__NO_SSE)
			this->reg = _mm_add_ps(this->reg, _mm_set_ps1(n));
			return *this;
#else
			return this->operator+=(n, n, n, n);
#endif
		}

		inline Vec4f32& operator-=(const float n) noexcept {
#if !defined(__POLAR__NO_SSE)
			this->reg = _mm_sub_ps(this->reg, _mm_set_ps1(n));
			return *this;
#else
			return this->operator-=(n, n, n, n);
#endif
		}

		inline Vec4f32& operator*=(const float n) noexcept {
#if !defined(__POLAR__NO_SSE)
			this->reg = _mm_mul_ps(this->reg, _mm_set_ps1(n));
			return *this;
#else
			return this->operator*=(n, n, n, n);
#endif
		}

		inline Vec4f32& operator/=(const float n) noexcept {
#if !defined(__POLAR__NO_SSE)
			this->reg = _mm_div_ps(this->reg, _mm_set_ps1(n));
			return *this;
#else
			return this->operator/=(n, n, n, n);
#endif
		}

		inline float GetLength() const noexcept {
			return std::sqrtf(this->x * this->x + this->y * this->y +
							  this->z * this->z + this->w * this->w);
		}

		inline void Clamp(const float min, const float max) noexcept {
			this->x = std::clamp(this->x, min, max);
			this->y = std::clamp(this->y, min, max);
			this->z = std::clamp(this->z, min, max);
			this->w = std::clamp(this->w, min, max);
		}

		static inline Vec4f32 Clamped(Vec4f32 v, const float min, const float max) {
			v.Clamp(min, max);
			return v;
		}

		inline void Normalize() noexcept;

		static inline Vec4f32 Normalized(Vec4f32 v) noexcept;

		static inline float DotProduct2D(const Vec4f32& a, const Vec4f32& b) noexcept;
		static inline float DotProduct3D(const Vec4f32& a, const Vec4f32& b) noexcept;
		static inline float DotProduct4D(const Vec4f32& a, const Vec4f32& b) noexcept;

		static inline Vec4f32 CrossProduct3D(const Vec4f32& a, const Vec4f32& b) noexcept;
		static inline Vec4f32 Reflected3D   (const Vec4f32& in, const Vec4f32& normal) noexcept;

#if !defined(__POLAR__NO_SSE)
		inline operator __m128() const noexcept { return reg; }
#endif  // !__POLAR__NO_SSE

	}; // struct Vec4f32

	inline Vec4f32 operator+(Vec4f32 a, const Vec4f32& b) noexcept { a += b; return a; }
	inline Vec4f32 operator-(Vec4f32 a, const Vec4f32& b) noexcept { a -= b; return a; }
	inline Vec4f32 operator*(Vec4f32 a, const Vec4f32& b) noexcept { a *= b; return a; }
	inline Vec4f32 operator/(Vec4f32 a, const Vec4f32& b) noexcept { a /= b; return a; }

	inline Vec4f32 operator+(Vec4f32 a, const float n) noexcept { a += n; return a; }
	inline Vec4f32 operator-(Vec4f32 a, const float n) noexcept { a -= n; return a; }
	inline Vec4f32 operator*(Vec4f32 a, const float n) noexcept { a *= n; return a; }
	inline Vec4f32 operator/(Vec4f32 a, const float n) noexcept { a /= n; return a; }

	inline Vec4f32 operator+(const float n, Vec4f32 a) noexcept { return a + n; }
	inline Vec4f32 operator-(const float n, Vec4f32 a) noexcept { return a - n; }
	inline Vec4f32 operator*(const float n, Vec4f32 a) noexcept { return a * n; }
	inline Vec4f32 operator/(const float n, Vec4f32 a) noexcept { return a / n; }

	inline void Vec4f32::Normalize() noexcept { (*this) /= this->GetLength(); }

	inline Vec4f32 Vec4f32::Normalized(Vec4f32 v) noexcept {
		v.Normalize();
		return v;
	}

	inline float Vec4f32::DotProduct2D(const Vec4f32& a, const Vec4f32& b) noexcept {
#if !defined(__POLAR__NO_SSE) && !defined(__POLAR__NO_SSE_4_1)
		return _mm_cvtss_f32(_mm_dp_ps(a.reg, b.reg, 0b00111111));
#else
		return a.x * b.x + a.y * b.y;
#endif
	}

	inline float Vec4f32::DotProduct3D(const Vec4f32& a, const Vec4f32& b) noexcept {
#if !defined(__POLAR__NO_SSE) && !defined(__POLAR__NO_SSE_4_1)
		return _mm_cvtss_f32(_mm_dp_ps(a.reg, b.reg, 0b01111111));
#else
		return a.x * b.x + a.y * b.y + a.z * b.z;
#endif
	}

	inline float Vec4f32::DotProduct4D(const Vec4f32& a, const Vec4f32& b) noexcept {
#if !defined(__POLAR__NO_SSE) && !defined(__POLAR__NO_SSE_4_1)
			return _mm_cvtss_f32(_mm_dp_ps(a.reg, b.reg, 0b11111111));
#else
			return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
#endif
	}

	inline Vec4f32 Vec4f32::CrossProduct3D(const Vec4f32& a, const Vec4f32& b) noexcept {
		const Vec4f32 tempA1(a.y, a.z, a.x, 0);
		const Vec4f32 tempB1(b.z, b.x, b.y, 0);
		const Vec4f32 tempA2(a.z, a.x, a.y, 0);
		const Vec4f32 tempB2(b.y, b.z, b.x, 0);

		return tempA1 * tempB1 - tempA2 * tempB2;
	}

	inline Vec4f32 Vec4f32::Reflected3D(const Vec4f32& in, const Vec4f32& normal) noexcept {
			return in - normal * 2 * Vec4f32::DotProduct3D(in, normal);
	}

	inline std::ostream& operator<<(std::ostream& stream, const Vec4f32& v) noexcept {
		stream << '(' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ')';

		return stream;
	}

}; // namespace PL

#endif // __POLAR__FILE_VECTOR_HPP