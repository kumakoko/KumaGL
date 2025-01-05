#ifndef jsm_type_defines_h__
#define jsm_type_defines_h__

namespace jsm
{
    using mat2 = std::array<float,4>;
    using ReadonlyMat2 = const mat2&;

    using mat2d = std::array<float,6>;
    using ReadonlyMat2d = const mat2d&;

    using mat3 = std::array<float, 8>;
    using ReadonlyMat3 = const mat3&;

    using mat4 = std::array<float, 16>;
    using ReadonlyMat4 = const mat4&;

    using vec2 = std::array<float, 2>; 
    using ReadonlyVec2 = const std::array<float, 2>&;

    using vec3 = std::array<float, 3>;
    using ReadonlyVec3 = const std::array<float, 3>&;
    
    using vec4 = std::array<float, 4>;
    using ReadonlyVec4 = const std::array<float, 4>&;

    using quat = std::array<float, 4>;
    using ReadonlyQuat = const std::array<float, 4>&;

    using quat2 = std::array<float, 8>;
    using ReadonlyQuat2 = const std::array<float, 8>&;
}

#endif // jsm_type_defines_h__