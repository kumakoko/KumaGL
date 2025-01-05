#include "../klib/kgl_lib_pch.h"
#include "jsm_common.h"
#include "jsm_mat4.h"


namespace jsm
{
    // Create an identity matrix
    mat4 createMat4()
    {
        mat4 out = { 0.0f,0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, };
        out[0] = 1.0f;
        out[5] = 1.0f;
        out[10] = 1.0f;
        out[15] = 1.0f;
        return out;
    }

    // Clone a matrix
    mat4 cloneMat4(const mat4& a)
    {
        mat4 out;
        std::copy(a.begin(), a.end(), out.begin());
        return out;
    }

    // Copy one matrix to another
    void copyMat4(mat4& out, const mat4& a)
    {
        std::copy(a.begin(), a.end(), out.begin());
    }

    // Transpose a matrix
    void transposeMat4(mat4& out, const mat4& a)
    {
        if (&out == &a)
        {
            float a01 = a[1], a02 = a[2], a03 = a[3];
            float a12 = a[6], a13 = a[7];
            float a23 = a[11];

            out[1] = a[4];
            out[2] = a[8];
            out[3] = a[12];
            out[4] = a01;
            out[6] = a[9];
            out[7] = a[13];
            out[8] = a02;
            out[9] = a12;
            out[11] = a[14];
            out[12] = a03;
            out[13] = a13;
            out[14] = a23;
        }
        else
        {
            out[0] = a[0]; out[1] = a[4]; out[2] = a[8];  out[3] = a[12];
            out[4] = a[1]; out[5] = a[5]; out[6] = a[9];  out[7] = a[13];
            out[8] = a[2]; out[9] = a[6]; out[10] = a[10]; out[11] = a[14];
            out[12] = a[3]; out[13] = a[7]; out[14] = a[11]; out[15] = a[15];
        }
    }

    void multiplyMat4(mat4& out, const mat4& a, const mat4& b)
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                out[i * 4 + j] = a[i * 4] * b[j] +
                    a[i * 4 + 1] * b[j + 4] +
                    a[i * 4 + 2] * b[j + 8] +
                    a[i * 4 + 3] * b[j + 12];
            }
        }
    }

    // Create a matrix from a quaternion
    mat4 fromQuat(const std::vector<float>& q)
    {
        mat4 out = { 0.0f,0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, };
        float x = q[0], y = q[1], z = q[2], w = q[3];
        float x2 = x + x, y2 = y + y, z2 = z + z;
        float xx = x * x2, xy = x * y2, xz = x * z2;
        float yy = y * y2, yz = y * z2, zz = z * z2;
        float wx = w * x2, wy = w * y2, wz = w * z2;

        out[0] = 1.0f - (yy + zz);
        out[1] = xy + wz;
        out[2] = xz - wy;
        out[4] = xy - wz;
        out[5] = 1.0f - (xx + zz);
        out[6] = yz + wx;
        out[8] = xz + wy;
        out[9] = yz - wx;
        out[10] = 1.0f - (xx + yy);
        out[15] = 1.0f;

        return out;
    }

    // Utility function to print a mat4
    void printMat4(const mat4& m)
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                //std::cout << m[i * 4 + j] << " ";
            }
            //std::cout << "\n";
        }
    }

    //int main() {
    //    mat4 identity = createMat4();
    //    printMat4(identity);

    //    mat4 cloned = cloneMat4(identity);
    //    printMat4(cloned);

    //    mat4 transposed(16);
    //    transposeMat4(transposed, identity);
    //    printMat4(transposed);

    //    mat4 multiplied(16, 0.0f);
    //    multiplyMat4(multiplied, identity, identity);
    //    printMat4(multiplied);

    //    std::vector<float> quat = { 0, 0, 0, 1 };
    //    mat4 quaternionMatrix = fromQuat(quat);
    //    printMat4(quaternionMatrix);

    //    return 0;
    //}

}