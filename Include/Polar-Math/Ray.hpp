#ifndef __POLAR__FILE_RAY_HPP
#define __POLAR__FILE_RAY_HPP

#include <Polar-Math/Vector.hpp>

namespace PL {

    struct Ray4f32 {
        Vec4f32 origin;
        Vec4f32 direction;
    }; // struct Ray4f32

}; // namespace PL

#endif // __POLAR__FILE_RAY_HPP