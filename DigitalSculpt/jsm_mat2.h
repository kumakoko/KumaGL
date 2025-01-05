#ifndef jsm_mat2_h__
#define jsm_mat2_h__

#include "jsm_type_defines.h"

namespace jsm
{
    // 创建单位矩阵
    mat2 mat2_create();

    // 克隆一个现有矩阵
    mat2 mat2_clone(ReadonlyMat2 a);

    // 复制矩阵
    mat2& mat2_copy(mat2& out, ReadonlyMat2 a);

    // 设置为单位矩阵
    mat2& identity(mat2& out);

    // 根据给定值创建矩阵
    mat2 mat2_fromValues(float m00, float m01, float m10, float m11);

    // 设置矩阵值
    mat2& mat2_set(mat2& out, float m00, float m01, float m10, float m11);

    // 矩阵转置
    mat2& transpose(mat2& out, ReadonlyMat2 a);

    // 矩阵求逆
    mat2& invert(mat2& out, ReadonlyMat2 a);

    // 矩阵伴随矩阵
    mat2& adjoint(mat2& out, ReadonlyMat2 a);

    // 计算行列式
    float determinant(ReadonlyMat2 a);

    // 矩阵乘法
    mat2& mat2_multiply(mat2& out, ReadonlyMat2 a, ReadonlyMat2 b);

    // 矩阵旋转
    mat2& rotate(mat2& out, ReadonlyMat2 a, float rad);

    // 矩阵缩放
    mat2& scale(mat2& out, ReadonlyMat2 a, ReadonlyVec2 v);

    // 从旋转角度创建矩阵
    mat2& fromRotation(mat2& out, float rad);

    // 从缩放向量创建矩阵
    mat2& fromScaling(mat2& out, ReadonlyVec2 v);

    // 打印矩阵为字符串
    std::string mat2_str(ReadonlyMat2 a);
}

#endif // jsm_mat2_h__
