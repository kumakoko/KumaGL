#ifndef jsm_mat4_h__
#define jsm_mat4_h__

#include "jsm_type_defines.h"

namespace jsm
{
    // Create an identity matrix
    mat4 createMat4();

    // Clone a matrix
    mat4 cloneMat4(const mat4& a);

    // Copy one matrix to another
    void copyMat4(mat4& out, const mat4& a);

    // Transpose a matrix
    void transposeMat4(mat4& out, const mat4& a);

    // Multiply two matrices
    void multiplyMat4(mat4& out, const mat4& a, const mat4& b);

    // Create a matrix from a quaternion
    mat4 fromQuat(const std::vector<float>& q);

    // Utility function to print a mat4
    void printMat4(const mat4& m);
}
#endif // jsm_mat4_h__
