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

#define __POLAR__DEFINE_POINT_COMPONENT_IN_STRUCT_OPERATORS(_N)																	 \
PointComponents() {  }																											 \
																																 \
PointComponents(const std::initializer_list<_T>& il) {																			 \
	std::memcpy(this->arr.data(), il.begin(), sizeof(_T) * std::max((size_t)_N, il.size()));									 \
}																																 \
																																 \
template <typename _U>																											 \
PointComponents(const PointComponents<_U, _N>& o) {																				 \
	std::memcpy(this->arr.data(), o.arr.data(), sizeof(_T) * _N);																 \
}																																 \
																																 \
template <typename _U>																											 \
PointComponents<_T, _N>& operator+=(const PointComponents<_U, _N>& o) noexcept {												 \
	size_t index = 0u;																											 \
	std::transform(this->arr.begin(), this->arr.end(), this->arr.begin(), [&o, &index](const _T& x) { return x + o.arr[index++]; });		 \
	return *this;																												 \
}																																 \
																																 \
template <typename _U>																											 \
PointComponents<_T, _N>& operator-=(const PointComponents<_U, _N>& o) noexcept {												 \
	size_t index = 0u;																											 \
	std::transform(this->arr.begin(), this->arr.end(), this->arr.begin(), [&o, &index](const _T& x) { return x - o.arr[index++]; });		 \
	return *this;																												 \
}																																 \
																																 \
template <typename _U>																											 \
PointComponents<_T, _N>& operator*=(const PointComponents<_U, _N>& o) noexcept {												 \
	size_t index = 0u;																											 \
	std::transform(this->arr.begin(), this->arr.end(), this->arr.begin(), [&o, &index](const _T& x) { return x * o.arr[index++]; });		 \
	return *this;																												 \
}																																 \
																																 \
template <typename _U>																											 \
PointComponents<_T, _N>& operator/=(const PointComponents<_U, _N>& o) noexcept {												 \
	size_t index = 0u;																											 \
	std::transform(this->arr.begin(), this->arr.end(), this->arr.begin(), [&o, &index](const _T& x) { return x / o.arr[index++]; });		 \
	return *this;																												 \
}																																 \

namespace PL {

    template <typename _T, size_t _N>
	struct PointComponents {  };

	template <typename _T, size_t _N> using Point = PointComponents<_T, _N>;
	template <typename _T, size_t _N> using Vec   = PointComponents<_T, _N>;

	template <typename _T> using Point1 = PointComponents<_T, 1u>;
	template <typename _T> using Point2 = PointComponents<_T, 2u>;
	template <typename _T> using Point3 = PointComponents<_T, 3u>;
	template <typename _T> using Point4 = PointComponents<_T, 4u>;

	template <typename _T> using Vec1 = PointComponents<_T, 1u>;
	template <typename _T> using Vec2 = PointComponents<_T, 2u>;
	template <typename _T> using Vec3 = PointComponents<_T, 3u>;
	template <typename _T> using Vec4 = PointComponents<_T, 4u>;

	template <typename _T, typename _U, size_t _N>
	inline auto operator+(const Vec<_T, _N>& a, const Vec<_U, _N>& b) noexcept -> Vec<decltype(a.x + b.x), _N>;

	template <typename _T, typename _U, size_t _N>
	inline auto operator-(const Vec<_T, _N>& a, const Vec<_U, _N>& b) noexcept -> Vec<decltype(a.x - b.x), _N>;

	template <typename _T, typename _U, size_t _N>
	inline auto operator*(const Vec<_T, _N>& a, const Vec<_U, _N>& b) noexcept -> Vec<decltype(a.x * b.x), _N>;

	template <typename _T, typename _U, size_t _N>
	inline auto operator/(const Vec<_T, _N>& a, const Vec<_U, _N>& b) noexcept -> Vec<decltype(a.x / b.x), _N>;

	template <typename _T> struct PointComponents<_T, 1u> {
		union {
			struct { _T x; };
			std::array<_T, 1u> arr;
		};

		__POLAR__DEFINE_POINT_COMPONENT_IN_STRUCT_OPERATORS(1u)
	}; // struct PointComponents<_T, 1u>

	template <typename _T> struct PointComponents<_T, 2u> {
		union {
			struct { _T x, y; };
			std::array<_T, 2u> arr;
		};

		__POLAR__DEFINE_POINT_COMPONENT_IN_STRUCT_OPERATORS(2u)
	}; // struct PointComponents<_T, 2u>

	template <typename _T> struct PointComponents<_T, 3u> {
		union {
			struct { _T x, y, z; };
			std::array<_T, 3u> arr;
		};

		__POLAR__DEFINE_POINT_COMPONENT_IN_STRUCT_OPERATORS(3u)
	}; // struct PointComponents<_T, 3u>

	template <typename _T> struct PointComponents<_T, 4u> {
		union {
			_T x, y, z, w;
			std::array<_T, 4u> arr;
		};

		__POLAR__DEFINE_POINT_COMPONENT_IN_STRUCT_OPERATORS(4u)
	}; // struct PointComponents<_T, 4u> 

	template <typename _T, typename _U, size_t _N>
	inline bool operator==(const Vec<_T, _N>& a, const Vec<_U, _N>& b) noexcept {
		if constexpr (!std::is_same_v<_T, _U>) {
			return false;
		} else {
			return std::memcmp(a.arr.data(), b.arr.data(), _N * sizeof(_T)) == 0;
		}
	}

	template <typename _T, typename _U, size_t _N>
	inline bool operator!=(const Vec<_T, _N>& a, const Vec<_U, _N>& b) noexcept {
		if constexpr (std::is_same_v<_T, _U>) {
			return true;
		} else {
			return std::memcmp(a.arr.data(), b.arr.data(), _N * sizeof(_T)) != 0;
		}
	}

	template <typename _T, typename _U, size_t _N>
	inline auto operator+(const Vec<_T, _N>& a, const Vec<_U, _N>& b) noexcept -> Vec<decltype(a.x + b.x), _N> {
		size_t index = 0u;
		Vec<decltype(a.x + b.x), _N> r(a);
		std::transform(a.arr.begin(), a.arr.end(), r.arr.begin(), [&b, &index](const _T& n) { return n + b.arr[index++]; });
		return r;
	}

	template <typename _T, typename _U, size_t _N>
	inline auto operator-(const Vec<_T, _N>& a, const Vec<_U, _N>& b) noexcept -> Vec<decltype(a.x - b.x), _N> {
		size_t index = 0u;
		Vec<decltype(a.x - b.x), _N> r(a);
		std::transform(a.arr.begin(), a.arr.end(), r.arr.begin(), [&b, &index](const _T& n) { return n - b.arr[index++]; });
		return r;
	}

	template <typename _T, typename _U, size_t _N>
	inline auto operator*(const Vec<_T, _N>& a, const Vec<_U, _N>& b) noexcept -> Vec<decltype(a.x * b.x), _N> {
		size_t index = 0u;
		Vec<decltype(a.x * b.x), _N> r(a);
		std::transform(a.arr.begin(), a.arr.end(), r.arr.begin(), [&b, &index](const _T& n) { return n * b.arr[index++]; });
		return r;
	}

	template <typename _T, typename _U, size_t _N>
	inline auto operator/(const Vec<_T, _N>& a, const Vec<_U, _N>& b) noexcept -> Vec<decltype(a.x / b.x), _N> {
		size_t index = 0u;
		Vec<decltype(a.x / b.x), _N> r(a);
		std::transform(a.arr.begin(), a.arr.end(), r.arr.begin(), [&b, &index](const _T& n) { return n / b.arr[index++]; });
		return r;
	}

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

	inline bool operator==(const Vec4f32& a, const Vec4f32& b) noexcept {
#if !defined(__POLAR__NO_SSE)
		return _mm_movemask_ps(_mm_cmpeq_ps(a.reg, b.reg)) == 0xFFFFFFFF;
#else
		return (a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w);
#endif
	}

	inline bool operator!=(const Vec4f32& a, const Vec4f32& b) noexcept {
#if !defined(__POLAR__NO_SSE)
		return _mm_movemask_ps(_mm_cmpeq_ps(a.reg, b.reg)) != 0xFFFFFFFF;
#else
		return (a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w);
#endif
	}

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

#undef __POLAR__DEFINE_POINT_COMPONENT_IN_STRUCT_OPERATORS

#endif // __POLAR__FILE_VECTOR_HPP