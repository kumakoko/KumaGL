/**************************************************************************************************************************
Copyright(C) 2014-2018 www.xionggf.com

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
#include "../klib/kgl_lib_pch.h"

#include "constants.h"

//const uint32_t SPHERE_LODS = 4;
//const uint32_t INDIRECT_BUFFERS = 4;


//const uint32_t SPHERE_LODS = 4;
const uint32_t DEPTH_SIZE = 256;
const uint32_t DEPTH_SIZE_LOG2 = 8;
const uint32_t GROUP_SIZE_AABB = 64;

const uint32_t PHYSICS_GROUP_SIZE = 128;
//const uint32_t INDIRECT_BUFFERS = 4;

// Spread our spheres out in three dimensions.
const uint32_t SPHERE_INSTANCES_X = 24;
const uint32_t SPHERE_INSTANCES_Y = 24;
const uint32_t SPHERE_INSTANCES_Z = 24;
const uint32_t SPHERE_INSTANCES = (SPHERE_INSTANCES_X * SPHERE_INSTANCES_Y * SPHERE_INSTANCES_Z);

const float  SPHERE_RADIUS = 0.30f;

// Defines how densely spheres should be tesselated (offline) at each LOD level.
const uint32_t SPHERE_VERT_PER_CIRC_LOD0 = 24;
const uint32_t SPHERE_VERT_PER_CIRC_LOD1 = 20;
const uint32_t SPHERE_VERT_PER_CIRC_LOD2 = 16;
const uint32_t SPHERE_VERT_PER_CIRC_LOD3 = 12;

// We use fixed uniform locations in the shaders (GLES 3.1 feature).
const uint32_t UNIFORM_MVP_LOCATION = 0;
const uint32_t UNIFORM_COLOR_LOCATION = 1;
const uint32_t UNIFORM_LIGHT_DIR_LOCATION = 2;

const float Z_NEAR = 1.0f;
const float Z_FAR = 500.0f;