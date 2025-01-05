// 2x3的矩阵
#ifndef jsm_mat2d_h__
#define jsm_mat2d_h__

#include "jsm_type_defines.h"

namespace jsm
{
    // 创建单位矩阵
    mat2d create();

    // 克隆矩阵
    mat2d clone(const mat2d& a);

    // 复制矩阵
    mat2d& copy(mat2d& out, const mat2d& a);

    // 设置为单位矩阵
    mat2d& identity(mat2d& out);

    // 创建矩阵
    mat2d fromValues(float a, float b, float c, float d, float tx, float ty);

    // 设置矩阵
    mat2d& set(mat2d& out, float a, float b, float c, float d, float tx, float ty);

    // 矩阵求逆
    mat2d& invert(mat2d& out, const mat2d& a);

    // 计算行列式
    float determinant(const mat2d& a);

    // 矩阵相乘
    mat2d& multiply(mat2d& out, const mat2d& a, const mat2d& b);

    // 矩阵旋转
    mat2d& rotate(mat2d& out, const mat2d& a, float rad);

    // 矩阵缩放
    mat2d& scale(mat2d& out, const mat2d& a, const vec2& v);

    // 矩阵平移
    mat2d& translate(mat2d& out, const mat2d& a, const vec2& v);

    // 从旋转角度创建矩阵
    mat2d fromRotation(float rad);

    // 从缩放向量创建矩阵
    mat2d fromScaling(const vec2& v);

    // 从平移向量创建矩阵
    mat2d fromTranslation(const vec2& v);

    // 转为字符串
    std::string str(const mat2d& a);
}

#endif // jsm_mat2d_h__