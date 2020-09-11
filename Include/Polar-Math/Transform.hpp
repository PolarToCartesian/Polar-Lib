#ifndef __POLAR__FILE_TRANSFORM_HPP
#define __POLAR__FILE_TRANSFORM_HPP

#include <Polar-Math/Matrix.hpp>

namespace PL {

    struct Transform {
	private:
		Vec4f32 m_rotation;
		Vec4f32 m_translation;

		Mat4x4f32 m_transform = Mat4x4f32::MakeIdentity();

	public:
		Transform() = default;

		[[nodiscard]] inline Vec4f32 GetRotation()    const noexcept { return this->m_rotation;    }
		[[nodiscard]] inline Vec4f32 GetTranslation() const noexcept { return this->m_translation; }

		inline void SetRotation   (const Vec4f32 &rotation)    noexcept { this->m_rotation    = rotation;    }
		inline void SetTranslation(const Vec4f32 &translation) noexcept { this->m_translation = translation; }

		inline void Rotate   (const Vec4f32 &delta) noexcept { this->m_rotation    += delta; }
		inline void Translate(const Vec4f32 &delta) noexcept { this->m_translation += delta; }

		[[nodiscard]] inline Mat4x4f32 GetTransform()           const noexcept { return this->m_transform;                 }
		[[nodiscard]] inline Mat4x4f32 GetTransposedTransform() const noexcept { return Mat4x4f32::MakeTransposed(this->m_transform); }

		inline void CalculateTransform() noexcept {
			this->m_transform = Mat4x4f32::MakeTranslation(this->m_translation);
		}
	}; // struct Transform

}; // namespace PL

#endif // __POLAR__FILE_TRANSFORM_HPP