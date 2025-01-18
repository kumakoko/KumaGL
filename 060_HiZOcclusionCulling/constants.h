/**************************************************************************************************************************
Copyright(C) 2014-2025 www.xionggf.com

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute,sublicense, and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM,OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************************************/
#ifndef constants_h__
#define constants_h__

#include <cstdint>

constexpr uint32_t SPHERE_LODS = 4;
constexpr uint32_t INDIRECT_BUFFERS = 4; // 间接绘制缓冲区的个数

extern const uint32_t DEPTH_SIZE;
extern const uint32_t DEPTH_SIZE_LOG2;
extern const uint32_t GROUP_SIZE_AABB;

extern const uint32_t PHYSICS_GROUP_SIZE;

// Spread our spheres out in three dimensions.
extern const uint32_t SPHERE_INSTANCES_X;
extern const uint32_t SPHERE_INSTANCES_Y;
extern const uint32_t SPHERE_INSTANCES_Z;
extern const uint32_t SPHERE_INSTANCES;

extern const float SPHERE_RADIUS;

// Defines how densely spheres should be tesselated (offline) at each LOD level.
extern const uint32_t SPHERE_VERT_PER_CIRC_LOD0;
extern const uint32_t SPHERE_VERT_PER_CIRC_LOD1;
extern const uint32_t SPHERE_VERT_PER_CIRC_LOD2;
extern const uint32_t SPHERE_VERT_PER_CIRC_LOD3;

// We use fixed uniform locations in the shaders (GLES 3.1 feature).
extern const uint32_t UNIFORM_MVP_LOCATION;
extern const uint32_t UNIFORM_COLOR_LOCATION;
extern const uint32_t UNIFORM_LIGHT_DIR_LOCATION;

extern const float Z_NEAR;
extern const float Z_FAR;

#endif // constants_h__