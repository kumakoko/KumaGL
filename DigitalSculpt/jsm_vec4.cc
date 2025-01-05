#include "../klib/kgl_lib_pch.h"
#include "jsm_common.h"
#include "jsm_vec4.h"

namespace jsm
{
    // 创建一个空的 vec4
    vec4 create()
    {
        vec4 v = { 0.0f, 0.0f, 0.0f, 0.0f }; // 初始化为零向量
        return v;
    }

    // 克隆一个现有的 vec4
    vec4 clone(ReadonlyVec4 a) {
        return vec4{ a[0], a[1], a[2], a[3] };
    }

    // 根据给定值创建 vec4
    vec4 fromValues(float x, float y, float z, float w)
    {
        return vec4{ x, y, z, w };
    }

    // 拷贝 vec4 的值
    vec4& copy(vec4& out, ReadonlyVec4 a)
    {
        out[0] = a[0];
        out[1] = a[1];
        out[2] = a[2];
        out[3] = a[3];
        return out;
    }

    // 设置 vec4 的值
    vec4& set(vec4& out, float x, float y, float z, float w)
    {
        out[0] = x;
        out[1] = y;
        out[2] = z;
        out[3] = w;
        return out;
    }

    // vec4 加法
    vec4& add(vec4& out, ReadonlyVec4 a, ReadonlyVec4 b)
    {
        out[0] = a[0] + b[0];
        out[1] = a[1] + b[1];
        out[2] = a[2] + b[2];
        out[3] = a[3] + b[3];
        return out;
    }

    // vec4 减法
    vec4& subtract(vec4& out, ReadonlyVec4 a, ReadonlyVec4 b)
    {
        out[0] = a[0] - b[0];
        out[1] = a[1] - b[1];
        out[2] = a[2] - b[2];
        out[3] = a[3] - b[3];
        return out;
    }

    // vec4 乘法
    vec4& multiply(vec4& out, ReadonlyVec4 a, ReadonlyVec4 b)
    {
        out[0] = a[0] * b[0];
        out[1] = a[1] * b[1];
        out[2] = a[2] * b[2];
        out[3] = a[3] * b[3];
        return out;
    }

    // vec4 除法
    vec4& divide(vec4& out, ReadonlyVec4 a, ReadonlyVec4 b)
    {
        out[0] = a[0] / b[0];
        out[1] = a[1] / b[1];
        out[2] = a[2] / b[2];
        out[3] = a[3] / b[3];
        return out;
    }

    // 标准化 vec4
    vec4& normalize(vec4& out, ReadonlyVec4 a)
    {
        float len = a[0] * a[0] + a[1] * a[1] + a[2] * a[2] + a[3] * a[3];

        if (len > 0)
        {
            len = 1.0f / std::sqrt(len);
            out[0] = a[0] * len;
            out[1] = a[1] * len;
            out[2] = a[2] * len;
            out[3] = a[3] * len;
        }
        return out;
    }

    // 点积
    float dot(ReadonlyVec4 a, ReadonlyVec4 b)
    {
        return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
    }

    // 检查两个 vec4 是否完全相等
    bool exactEquals(ReadonlyVec4 a, ReadonlyVec4 b)
    {
        return a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3];
    }

    // 检查两个 vec4 是否近似相等
    bool equals(ReadonlyVec4 a, ReadonlyVec4 b)
    {
        return
            std::abs(a[0] - b[0]) <= EPSILON * max(1.0f, std::abs(a[0]), std::abs(b[0])) &&
            std::abs(a[1] - b[1]) <= EPSILON * max(1.0f, std::abs(a[1]), std::abs(b[1])) &&
            std::abs(a[2] - b[2]) <= EPSILON * max(1.0f, std::abs(a[2]), std::abs(b[2])) &&
            std::abs(a[3] - b[3]) <= EPSILON * max(1.0f, std::abs(a[3]), std::abs(b[3]));
    }

    // 打印 vec4 为字符串
    std::string str(ReadonlyVec4 a)
    {
        return "vec4(" + std::to_string(a[0]) + ", " + std::to_string(a[1]) + ", " + std::to_string(a[2]) + ", " + std::to_string(a[3]) + ")";
    }

    //int main() {
    //    // 示例使用
    //    vec4 v1 = fromValues(1.0f, 2.0f, 3.0f, 4.0f);
    //    vec4 v2 = fromValues(4.0f, 5.0f, 6.0f, 7.0f);
    //    vec4 result = create();

    //    add(result, v1, v2);
    //    std::cout << "Result of addition: " << str(result) << std::endl;

    //    normalize(result, result);
    //    std::cout << "Normalized vector: " << str(result) << std::endl;

    //    return 0;
    //}

}