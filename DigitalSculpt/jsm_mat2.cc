#include "../klib/kgl_lib_pch.h"
#include "jsm_common.h"
#include "jsm_mat2.h"

namespace jsm
{
    // 创建单位矩阵
    mat2 mat2_0create()
    {
        return mat2{ 1, 0, 0, 1 };
    }

    // 克隆一个现有矩阵
    mat2 mat2_clone(ReadonlyMat2 a)
    {
        return mat2{ a[0], a[1], a[2], a[3] };
    }

    // 复制矩阵
    mat2& mat2_copy(mat2& out, ReadonlyMat2 a)
    {
        out[0] = a[0];
        out[1] = a[1];
        out[2] = a[2];
        out[3] = a[3];
        return out;
    }

    // 设置为单位矩阵
    mat2& identity(mat2& out)
    {
        out[0] = 1;
        out[1] = 0;
        out[2] = 0;
        out[3] = 1;
        return out;
    }

    // 根据给定值创建矩阵
    mat2 mat2_fromValues(float m00, float m01, float m10, float m11)
    {
        return mat2{ m00, m01, m10, m11 };
    }

    // 设置矩阵值
    mat2& mat2_set(mat2& out, float m00, float m01, float m10, float m11)
    {
        out[0] = m00;
        out[1] = m01;
        out[2] = m10;
        out[3] = m11;
        return out;
    }

    // 矩阵转置
    mat2& transpose(mat2& out, ReadonlyMat2 a)
    {
        if (&out == &a)
        {
            std::swap(out[1], out[2]);
        }
        else
        {
            out[0] = a[0];
            out[1] = a[2];
            out[2] = a[1];
            out[3] = a[3];
        }

        return out;
    }

    // 矩阵求逆
    mat2& invert(mat2& out, ReadonlyMat2 a)
    {
        float det = a[0] * a[3] - a[2] * a[1];

        if (std::abs(det) < EPSILON)
        {
            throw std::runtime_error("Matrix is not invertible.");
        }

        det = 1.0f / det;

        out[0] = a[3] * det;
        out[1] = -a[1] * det;
        out[2] = -a[2] * det;
        out[3] = a[0] * det;

        return out;
    }

    // 矩阵伴随矩阵
    mat2& adjoint(mat2& out, ReadonlyMat2 a)
    {
        out[0] = a[3];
        out[1] = -a[1];
        out[2] = -a[2];
        out[3] = a[0];
        return out;
    }

    // 计算行列式
    float determinant(ReadonlyMat2 a)
    {
        return a[0] * a[3] - a[2] * a[1];
    }

    // 矩阵乘法
    mat2& mat2_multiply(mat2& out, ReadonlyMat2 a, ReadonlyMat2 b)
    {
        out[0] = a[0] * b[0] + a[2] * b[1];
        out[1] = a[1] * b[0] + a[3] * b[1];
        out[2] = a[0] * b[2] + a[2] * b[3];
        out[3] = a[1] * b[2] + a[3] * b[3];
        return out;
    }
    
    // 矩阵旋转
    mat2& rotate(mat2& out, ReadonlyMat2 a, float rad)
    {
        float s = std::sin(rad);
        float c = std::cos(rad);

        out[0] = a[0] * c + a[2] * s;
        out[1] = a[1] * c + a[3] * s;
        out[2] = a[0] * -s + a[2] * c;
        out[3] = a[1] * -s + a[3] * c;

        return out;
    }

    // 矩阵缩放
    mat2& scale(mat2& out, ReadonlyMat2 a, ReadonlyVec2 v)
    {
        out[0] = a[0] * v[0];
        out[1] = a[1] * v[0];
        out[2] = a[2] * v[1];
        out[3] = a[3] * v[1];
        return out;
    }

    // 从旋转角度创建矩阵
    mat2& fromRotation(mat2& out, float rad)
    {
        float s = std::sin(rad);
        float c = std::cos(rad);
        out[0] = c;
        out[1] = s;
        out[2] = -s;
        out[3] = c;
        return out;
    }

    // 从缩放向量创建矩阵
    mat2& fromScaling(mat2& out, ReadonlyVec2 v)
    {
        out[0] = v[0];
        out[1] = 0;
        out[2] = 0;
        out[3] = v[1];
        return out;
    }

    // 打印矩阵为字符串
    std::string mat2_str(ReadonlyMat2 a)
    {
        return "mat2(" + std::to_string(a[0]) + ", " + std::to_string(a[1]) + ", " +
            std::to_string(a[2]) + ", " + std::to_string(a[3]) + ")";
    }
}