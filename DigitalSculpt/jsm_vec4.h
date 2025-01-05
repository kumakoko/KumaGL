#ifndef jsm_vec4_h__
#define jsm_vec4_h__

#include "jsm_type_defines.h"

namespace jsm
{
    // 创建一个空的 vec4
    vec4 create();

    // 克隆一个现有的 vec4
    vec4 clone(ReadonlyVec4 a);

    // 根据给定值创建 vec4
    vec4 fromValues(float x, float y, float z, float w);

    // 拷贝 vec4 的值
    vec4& copy(vec4& out, ReadonlyVec4 a);

    // 设置 vec4 的值
    vec4& set(vec4& out, float x, float y, float z, float w);

    // vec4 加法
    vec4& add(vec4& out, ReadonlyVec4 a, ReadonlyVec4 b);

    // vec4 减法
    vec4& subtract(vec4& out, ReadonlyVec4 a, ReadonlyVec4 b);

    // vec4 乘法
    vec4& multiply(vec4& out, ReadonlyVec4 a, ReadonlyVec4 b);

    // vec4 除法
    vec4& divide(vec4& out, ReadonlyVec4 a, ReadonlyVec4 b);

    // 标准化 vec4
    vec4& normalize(vec4& out, ReadonlyVec4 a);

    // 点积
    float dot(ReadonlyVec4 a, ReadonlyVec4 b);
    // 检查两个 vec4 是否完全相等
    bool exactEquals(ReadonlyVec4 a, ReadonlyVec4 b);

    // 检查两个 vec4 是否近似相等
    bool equals(ReadonlyVec4 a, ReadonlyVec4 b);

    // 打印 vec4 为字符串
    std::string str(ReadonlyVec4 a);
}

#endif // jsm_vec4_h__