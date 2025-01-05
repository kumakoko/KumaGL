#ifndef jsm_mat3_h__
#define jsm_mat3_h__

#include "jsm_type_defines.h"

namespace jsm
{
    typedef std::array<float,9> mat3;
   // typedef std::vector<float> mat4;
    typedef std::vector<float> vec2;
    typedef std::array<float,4> quat;

    // 创建一个单位矩阵
    mat3 createMat3() {
        return mat3{ 1.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 1.0f };
    }

    // 从mat4中提取一个mat3
    mat3 fromMat4(const mat4& a) {
        return mat3{ a[0], a[1], a[2],
                    a[4], a[5], a[6],
                    a[8], a[9], a[10] };
    }

    // 克隆一个mat3
    mat3 cloneMat3(const mat3& a) {
        return mat3{ a[0], a[1], a[2],
                    a[3], a[4], a[5],
                    a[6], a[7], a[8] };
    }

    // 将一个mat3设置为单位矩阵
    void identityMat3(mat3& out) {
        out = mat3{ 1.0f, 0.0f, 0.0f,
                   0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 1.0f };
    }

    // 转置一个mat3
    void transposeMat3(mat3& out, const mat3& a) {
        out[0] = a[0];
        out[1] = a[3];
        out[2] = a[6];
        out[3] = a[1];
        out[4] = a[4];
        out[5] = a[7];
        out[6] = a[2];
        out[7] = a[5];
        out[8] = a[8];
    }

    // 矩阵相乘
    void multiplyMat3(mat3& out, const mat3& a, const mat3& b) {
        out[0] = b[0] * a[0] + b[1] * a[3] + b[2] * a[6];
        out[1] = b[0] * a[1] + b[1] * a[4] + b[2] * a[7];
        out[2] = b[0] * a[2] + b[1] * a[5] + b[2] * a[8];

        out[3] = b[3] * a[0] + b[4] * a[3] + b[5] * a[6];
        out[4] = b[3] * a[1] + b[4] * a[4] + b[5] * a[7];
        out[5] = b[3] * a[2] + b[4] * a[5] + b[5] * a[8];

        out[6] = b[6] * a[0] + b[7] * a[3] + b[8] * a[6];
        out[7] = b[6] * a[1] + b[7] * a[4] + b[8] * a[7];
        out[8] = b[6] * a[2] + b[7] * a[5] + b[8] * a[8];
    }

    // 打印矩阵
    void printMat3(const mat3& m) {
        std::cout << "mat3(";
        for (size_t i = 0; i < 9; ++i) {
            std::cout << m[i];
            if (i < 8) std::cout << ", ";
        }
        std::cout << ")" << std::endl;
    }

    int main() {
        mat3 identity = createMat3();
        printMat3(identity);

        mat3 transposed;
        transposeMat3(transposed, identity);
        printMat3(transposed);

        return 0;
    }

}

#endif // jsm_mat3_h__