#ifndef __POLAR__FILE_MATRIX_HPP
#define __POLAR__FILE_MATRIX_HPP

#include <Polar-Math/Vector.hpp>

#if !defined(__POLAR__NO_AVX)
    #include <immintrin.h> 
#endif

#include <array>

namespace PL {

    class Mat4x4f32;
	inline Mat4x4f32 operator*(const Mat4x4f32& matA, const Mat4x4f32& matB) noexcept;

	class Mat4x4f32 {
	private:
		std::array<float, 16> m = { 0 };

	public:
		Mat4x4f32() = default;
		Mat4x4f32(const std::array<float, 16>& arr) noexcept : m(arr) {  }

		inline       float* Data()       noexcept { return this->m.data(); }
		inline const float* Data() const noexcept { return this->m.data(); }

		inline       float& operator[](const size_t i)       noexcept { return this->m[i]; }
		inline const float& operator[](const size_t i) const noexcept { return this->m[i]; }

		inline       float& operator()(const size_t r, const size_t c)       noexcept { return this->m[r * 4u + c]; }
		inline const float& operator()(const size_t r, const size_t c) const noexcept { return this->m[r * 4u + c]; }

		// --- Static Functions ---
		static inline Mat4x4f32 MakeZeros()    noexcept { return Mat4x4f32(); }

		static inline Mat4x4f32 MakeIdentity() noexcept {
			return Mat4x4f32(std::array<float, 16u>{
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			});
		}

		static inline Mat4x4f32 MakeRotationX(const float radX = 0) noexcept
		{
			const float sinX = std::sin(radX);
			const float cosX = std::cos(radX);

			return Mat4x4f32(std::array<float, 16u>{
				1, 0, 0, 0,
					0, cosX, -sinX, 0,
					0, sinX, cosX, 0,
					0, 0, 0, 1
			});
		}

		static inline Mat4x4f32 MakeRotationY(const float radY = 0) noexcept
		{
			const float sinY = std::sin(radY);
			const float cosY = std::cos(radY);

			return Mat4x4f32(std::array<float, 16u>{
				cosY, 0, sinY, 0,
				0,    1, 0,    0,
				-sinY, 0, cosY, 0,
				0, 0, 0, 1
			});
		}

		static inline Mat4x4f32 MakeRotationZ(const float radZ = 0) noexcept
		{
			const float sinZ = std::sin(radZ);
			const float cosZ = std::cos(radZ);

			return Mat4x4f32(std::array<float, 16u>{
				cosZ, -sinZ, 0, 0,
					sinZ, cosZ, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1
			});
		}

		static inline Mat4x4f32 MakeRotation(const float radX = 0, const float radY = 0, const float radZ = 0) noexcept
		{
			return MakeRotationX(radX) *
				   MakeRotationY(radY) *
				   MakeRotationZ(radZ);
		}

		static inline Mat4x4f32 MakeRotation(const Vec4f32& radians) noexcept
		{
			return Mat4x4f32::MakeRotation(radians.x, radians.y, radians.z);
		}

		static inline Mat4x4f32 MakeTranslation(const float x, const float y, const float z) noexcept
		{
			return Mat4x4f32(std::array<float, 16u>{
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				-x, -y, -z, 1,
			});
		}

		static inline Mat4x4f32 MakeTranslation(const Vec4f32& translation) noexcept
		{
			return Mat4x4f32::MakeTranslation(translation.x, translation.y, translation.z);
		}

		static inline Mat4x4f32 MakePerspective(const float zNear, const float zFar, const float fovRad, const float aspectRatio)
		{
			return Mat4x4f32(std::array<float, 16u> {
				aspectRatio* fovRad, 0, 0, 0,
				0, fovRad, 0, 0,
				0, 0, zFar / (zFar - zNear), 1,
				0, 0, (-zFar * zNear) / (zFar - zNear), 1,
			});
		}

		static inline Mat4x4f32 MakeScaling(const float scaleX = 1.0f, const float scaleY = 1.0f, const float scaleZ = 1.0f, const float scaleW = 1.0f) noexcept
		{
			return Mat4x4f32(std::array<float, 16u>{
				scaleX, 0, 0, 0,
				0, scaleY, 0, 0,
				0, 0, scaleZ, 0,
				0, 0, 0, scaleW
			});
		}

		static inline Mat4x4f32 MakeScaling(const Vec4f32& scale) noexcept
		{
			return Mat4x4f32::MakeScaling(scale.x, scale.y, scale.z, scale.w);
		}

		static inline Mat4x4f32 MakeTransposed(const Mat4x4f32& mat) noexcept
		{
			return Mat4x4f32(std::array<float, 16u>{
				mat(0, 0), mat(1, 0), mat(2, 0), mat(3, 0),
				mat(0, 1), mat(1, 1), mat(2, 1), mat(3, 1),
				mat(0, 2), mat(1, 2), mat(2, 2), mat(3, 2),
				mat(0, 3), mat(1, 3), mat(2, 3), mat(3, 3)
			});
		}

		// Thanks to: https://stackoverflow.com/questions/349050/calculating-a-lookat-matrix
		static inline Mat4x4f32 MakeLookAt(const Vec4f32& cameraPosition, const Vec4f32& focalPoint, const Vec4f32& upDirection) noexcept
		{
			const Vec4f32 zaxis = Vec4f32::Normalized(focalPoint - cameraPosition);
			const Vec4f32 xaxis = Vec4f32::Normalized(Vec4f32::CrossProduct(upDirection, zaxis));
			const Vec4f32 yaxis = Vec4f32::CrossProduct(zaxis, xaxis);

			const float m30 = -Vec4f32::DotProduct(xaxis, cameraPosition);
			const float m31 = -Vec4f32::DotProduct(yaxis, cameraPosition);
			const float m32 = -Vec4f32::DotProduct(zaxis, cameraPosition);

			return Mat4x4f32(std::array<float, 16u>{
				xaxis.x, yaxis.x, zaxis.x, 0,
				xaxis.y, yaxis.y, zaxis.y, 0,
				xaxis.z, yaxis.z, zaxis.z, 0,
				m30, m31, m32, 1
			});
		}
	}; // class Mat4x4f32

	inline Mat4x4f32 operator+(const Mat4x4f32& a, const Mat4x4f32& b) noexcept {
		Mat4x4f32 result;

#if !defined(__POLAR__NO_AVX)
		_mm256_store_ps(result.Data(), _mm256_add_ps(_mm256_load_ps(a.Data()), _mm256_load_ps(b.Data())));
		_mm256_store_ps(result.Data() + 8, _mm256_add_ps(_mm256_load_ps(a.Data() + 8), _mm256_load_ps(b.Data() + 8)));
#else
		for (size_t i = 0u; i < 16u; i++)
			result[i] = a[i] + b[i];
#endif

		return result;
	}

	inline Mat4x4f32 operator-(const Mat4x4f32& a, const Mat4x4f32& b) noexcept {
		Mat4x4f32 result;

#if !defined(__POLAR__NO_AVX)
		_mm256_store_ps(result.Data(), _mm256_sub_ps(_mm256_load_ps(a.Data()), _mm256_load_ps(b.Data())));
		_mm256_store_ps(result.Data() + 8, _mm256_sub_ps(_mm256_load_ps(a.Data() + 8), _mm256_load_ps(b.Data() + 8)));
#else
		for (size_t i = 0u; i < 16u; i++)
			result[i] = a[i] - b[i];
#endif

		return result;
	}

	inline Mat4x4f32 operator*(const Mat4x4f32& matA, const Mat4x4f32& matB) noexcept
	{
		Mat4x4f32 matResult;

		for (size_t r = 0u; r < 4u; r++) {
			for (size_t c = 0u; c < 4u; c++) {
				const Vec4f32 vecA = Vec4f32(matA(r, 0), matA(r, 1), matA(r, 2), matA(r, 3));
				const Vec4f32 vecB = Vec4f32(matB(0, c), matB(1, c), matB(2, c), matB(3, c));

				matResult(r, c) = Vec4f32::DotProduct(vecA, vecB);
			}
		}

		return matResult;
	}

	inline Vec4f32 operator*(const Mat4x4f32& mat, const Vec4f32& vec) noexcept
	{
		return Vec4f32(
			Vec4f32::DotProduct(vec, Vec4f32(mat(0, 0), mat(1, 0), mat(2, 0), mat(3, 0))),
			Vec4f32::DotProduct(vec, Vec4f32(mat(0, 1), mat(1, 1), mat(2, 1), mat(3, 1))),
			Vec4f32::DotProduct(vec, Vec4f32(mat(0, 2), mat(1, 2), mat(2, 2), mat(3, 2))),
			Vec4f32::DotProduct(vec, Vec4f32(mat(0, 3), mat(1, 3), mat(2, 3), mat(3, 3)))
		);
	}

}; // namespace PL

#endif // __POLAR__FILE_MATRIX_HPP