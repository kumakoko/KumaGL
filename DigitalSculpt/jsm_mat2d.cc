#include "../klib/kgl_lib_pch.h"
#include "jsm_common.h"
#include "jsm_mat2d.h"

namespace jsm
{
    // 创建单位矩阵
    mat2d create()
    {
        return mat2d{ 1, 0, 0, 1, 0, 0 };
    }

    // 克隆矩阵
    mat2d clone(const mat2d& a)
    {
        return mat2d{ a[0], a[1], a[2], a[3], a[4], a[5] };
    }

    // 复制矩阵
    mat2d& copy(mat2d& out, const mat2d& a)
    {
        out[0] = a[0];
        out[1] = a[1];
        out[2] = a[2];
        out[3] = a[3];
        out[4] = a[4];
        out[5] = a[5];
        return out;
    }

    // 设置为单位矩阵
    mat2d& identity(mat2d& out)
    {
        out[0] = 1; out[1] = 0;
        out[2] = 0; out[3] = 1;
        out[4] = 0; out[5] = 0;
        return out;
    }

    // 创建矩阵
    mat2d fromValues(float a, float b, float c, float d, float tx, float ty)
    {
        return mat2d{ a, b, c, d, tx, ty };
    }

    // 设置矩阵
    mat2d& set(mat2d& out, float a, float b, float c, float d, float tx, float ty)
    {
        out[0] = a; out[1] = b;
        out[2] = c; out[3] = d;
        out[4] = tx; out[5] = ty;
        return out;
    }

    // 矩阵求逆
    mat2d& invert(mat2d& out, const mat2d& a)
    {
        float det = a[0] * a[3] - a[1] * a[2];

        if (std::abs(det) < EPSILON)
        {
            throw std::runtime_error("Matrix is not invertible.");
        }

        det = 1.0f / det;

        out[0] = a[3] * det;
        out[1] = -a[1] * det;
        out[2] = -a[2] * det;
        out[3] = a[0] * det;
        out[4] = (a[2] * a[5] - a[3] * a[4]) * det;
        out[5] = (a[1] * a[4] - a[0] * a[5]) * det;

        return out;
    }

    // 计算行列式
    float determinant(const mat2d& a)
    {
        return a[0] * a[3] - a[1] * a[2];
    }

    // 矩阵相乘
    mat2d& multiply(mat2d& out, const mat2d& a, const mat2d& b)
    {
        out[0] = a[0] * b[0] + a[2] * b[1];
        out[1] = a[1] * b[0] + a[3] * b[1];
        out[2] = a[0] * b[2] + a[2] * b[3];
        out[3] = a[1] * b[2] + a[3] * b[3];
        out[4] = a[0] * b[4] + a[2] * b[5] + a[4];
        out[5] = a[1] * b[4] + a[3] * b[5] + a[5];
        return out;
    }

    // 矩阵旋转
    mat2d& rotate(mat2d& out, const mat2d& a, float rad)
    {
        float s = std::sin(rad);
        float c = std::cos(rad);

        out[0] = a[0] * c + a[2] * s;
        out[1] = a[1] * c + a[3] * s;
        out[2] = a[0] * -s + a[2] * c;
        out[3] = a[1] * -s + a[3] * c;
        out[4] = a[4];
        out[5] = a[5];

        return out;
    }

    // 矩阵缩放
    mat2d& scale(mat2d& out, const mat2d& a, const vec2& v)
    {
        out[0] = a[0] * v[0];
        out[1] = a[1] * v[0];
        out[2] = a[2] * v[1];
        out[3] = a[3] * v[1];
        out[4] = a[4];
        out[5] = a[5];
        return out;
    }

    // 矩阵平移
    mat2d& translate(mat2d& out, const mat2d& a, const vec2& v)
    {
        out[0] = a[0];
        out[1] = a[1];
        out[2] = a[2];
        out[3] = a[3];
        out[4] = a[0] * v[0] + a[2] * v[1] + a[4];
        out[5] = a[1] * v[0] + a[3] * v[1] + a[5];
        return out;
    }

    // 从旋转角度创建矩阵
    mat2d fromRotation(float rad)
    {
        float s = std::sin(rad);
        float c = std::cos(rad);
        return mat2d{ c, s, -s, c, 0, 0 };
    }

    // 从缩放向量创建矩阵
    mat2d fromScaling(const vec2& v)
    {
        return mat2d{ v[0], 0, 0, v[1], 0, 0 };
    }

    // 从平移向量创建矩阵
    mat2d fromTranslation(const vec2& v)
    {
        return mat2d{ 1, 0, 0, 1, v[0], v[1] };
    }

    // 转为字符串
    std::string str(const mat2d& a)
    {
        return "mat2d(" + std::to_string(a[0]) + ", " + std::to_string(a[1]) + ", " +
            std::to_string(a[2]) + ", " + std::to_string(a[3]) + ", " +
            std::to_string(a[4]) + ", " + std::to_string(a[5]) + ")";
    }

    //int main() {
    //    mat2d mat = create();
    //    vec2 v{ 2, 3 };

    //    mat2d scaled = fromScaling(v);
    //    mat2d rotated = fromRotation(3.14f / 4);

    //    mat2d result = create();
    //    multiply(result, scaled, rotated);

    //    std::cout << "Result: " << str(result) << std::endl;
    //    return 0;
    //}
}