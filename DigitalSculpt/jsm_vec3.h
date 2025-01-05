#ifndef jsm_vec3_h__
#define jsm_vec3_h__

#include "jsm_type_defines.h"

namespace jsm
{
    // 创建一个空的 vec3
    vec3 create();

    // 克隆一个现有的 vec3
    vec3 clone(ReadonlyVec3 a);

    // 计算 vec3 的长度
    float length(ReadonlyVec3 a);

    // 根据给定值创建 vec3
    vec3 fromValues(float x, float y, float z);

    // 拷贝 vec3 的值
    vec3& copy(vec3& out, ReadonlyVec3 a);

    // 设置 vec3 的值
    vec3& set(vec3& out, float x, float y, float z);

    // vec3 加法
    vec3& add(vec3& out, ReadonlyVec3 a, ReadonlyVec3 b);

    // vec3 减法
    vec3& subtract(vec3& out, ReadonlyVec3 a, ReadonlyVec3 b);

    // vec3 乘法
    vec3& multiply(vec3& out, ReadonlyVec3 a, ReadonlyVec3 b);

    // vec3 除法
    vec3& divide(vec3& out, ReadonlyVec3 a, ReadonlyVec3 b);

    // 向上取整
    vec3& ceil(vec3& out, ReadonlyVec3 a);

    // 向下取整
    vec3& floor(vec3& out, ReadonlyVec3 a);

    // 返回两个 vec3 的最小值
    vec3& min(vec3& out, ReadonlyVec3 a, ReadonlyVec3 b);

    // 返回两个 vec3 的最大值
    vec3& max(vec3& out, ReadonlyVec3 a, ReadonlyVec3 b);

    // 标准化 vec3
    vec3& normalize(vec3& out, ReadonlyVec3 a);

    // 计算两个 vec3 的点积
    float dot(ReadonlyVec3 a, ReadonlyVec3 b);

    // 计算两个 vec3 的叉积
    vec3& cross(vec3& out, ReadonlyVec3 a, ReadonlyVec3 b);

    // 检查两个 vec3 是否完全相等
    bool exactEquals(ReadonlyVec3 a, ReadonlyVec3 b);

    // 检查两个 vec3 是否近似相等
    bool equals(ReadonlyVec3 a, ReadonlyVec3 b);

    // 打印 vec3 为字符串
    std::string str(ReadonlyVec3 a);
}

#endif // jsm_vec3_h__