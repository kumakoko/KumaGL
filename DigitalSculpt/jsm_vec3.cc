#include "../klib/kgl_lib_pch.h"
#include "jsm_common.h"
#include "jsm_vec3.h"

namespace jsm
{
    // 创建一个空的 vec3
    vec3 create()
    {
        vec3 v = { 0.0f,0.0f,0.0f }; // 初始化为零向量
        return v;
    }

    // 克隆一个现有的 vec3
    vec3 clone(ReadonlyVec3 a)
    {
        return vec3{ a[0], a[1], a[2] };
    }

    // 计算 vec3 的长度
    float length(ReadonlyVec3 a)
    {
        return std::sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    }

    // 根据给定值创建 vec3
    vec3 fromValues(float x, float y, float z)
    {
        return vec3{ x, y, z };
    }

    // 拷贝 vec3 的值
    vec3& copy(vec3& out, ReadonlyVec3 a)
    {
        out[0] = a[0];
        out[1] = a[1];
        out[2] = a[2];
        return out;
    }

    // 设置 vec3 的值
    vec3& set(vec3& out, float x, float y, float z)
    {
        out[0] = x;
        out[1] = y;
        out[2] = z;
        return out;
    }

    // vec3 加法
    vec3& add(vec3& out, ReadonlyVec3 a, ReadonlyVec3 b)
    {
        out[0] = a[0] + b[0];
        out[1] = a[1] + b[1];
        out[2] = a[2] + b[2];
        return out;
    }

    // vec3 减法
    vec3& subtract(vec3& out, ReadonlyVec3 a, ReadonlyVec3 b)
    {
        out[0] = a[0] - b[0];
        out[1] = a[1] - b[1];
        out[2] = a[2] - b[2];
        return out;
    }

    // vec3 乘法
    vec3& multiply(vec3& out, ReadonlyVec3 a, ReadonlyVec3 b)
    {
        out[0] = a[0] * b[0];
        out[1] = a[1] * b[1];
        out[2] = a[2] * b[2];
        return out;
    }

    // vec3 除法
    vec3& divide(vec3& out, ReadonlyVec3 a, ReadonlyVec3 b)
    {
        out[0] = a[0] / b[0];
        out[1] = a[1] / b[1];
        out[2] = a[2] / b[2];
        return out;
    }

    // 向上取整
    vec3& ceil(vec3& out, ReadonlyVec3 a)
    {
        out[0] = std::ceil(a[0]);
        out[1] = std::ceil(a[1]);
        out[2] = std::ceil(a[2]);
        return out;
    }

    // 向下取整
    vec3& floor(vec3& out, ReadonlyVec3 a)
    {
        out[0] = std::floor(a[0]);
        out[1] = std::floor(a[1]);
        out[2] = std::floor(a[2]);
        return out;
    }

    // 返回两个 vec3 的最小值
    vec3& min(vec3& out, ReadonlyVec3 a, ReadonlyVec3 b)
    {
        out[0] = std::min(a[0], b[0]);
        out[1] = std::min(a[1], b[1]);
        out[2] = std::min(a[2], b[2]);
        return out;
    }

    // 返回两个 vec3 的最大值
    vec3& max(vec3& out, ReadonlyVec3 a, ReadonlyVec3 b)
    {
        out[0] = std::max(a[0], b[0]);
        out[1] = std::max(a[1], b[1]);
        out[2] = std::max(a[2], b[2]);
        return out;
    }

    // 标准化 vec3
    vec3& normalize(vec3& out, ReadonlyVec3 a)
    {
        float len = length(a);

        if (len > 0)
        {
            out[0] = a[0] / len;
            out[1] = a[1] / len;
            out[2] = a[2] / len;
        }
        return out;
    }

    // 计算两个 vec3 的点积
    float dot(ReadonlyVec3 a, ReadonlyVec3 b)
    {
        return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
    }

    // 计算两个 vec3 的叉积
    vec3& cross(vec3& out, ReadonlyVec3 a, ReadonlyVec3 b)
    {
        out[0] = a[1] * b[2] - a[2] * b[1];
        out[1] = a[2] * b[0] - a[0] * b[2];
        out[2] = a[0] * b[1] - a[1] * b[0];
        return out;
    }

    // 检查两个 vec3 是否完全相等
    bool exactEquals(ReadonlyVec3 a, ReadonlyVec3 b)
    {
        return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
    }

    // 检查两个 vec3 是否近似相等
    bool equals(ReadonlyVec3 a, ReadonlyVec3 b)
    {
        return std::abs(a[0] - b[0]) <= EPSILON * max(1.0f, std::abs(a[0]), std::abs(b[0])) &&
            std::abs(a[1] - b[1]) <= EPSILON * max(1.0f, std::abs(a[1]), std::abs(b[1])) &&
            std::abs(a[2] - b[2]) <= EPSILON * max(1.0f, std::abs(a[2]), std::abs(b[2]));
    }

    // 打印 vec3 为字符串
    std::string str(ReadonlyVec3 a) 
    {
        return "vec3(" + std::to_string(a[0]) + ", " + std::to_string(a[1]) + ", " + std::to_string(a[2]) + ")";
    }

    //int main() {
    //    // 示例使用
    //    vec3 v1 = fromValues(1.0f, 2.0f, 3.0f);
    //    vec3 v2 = fromValues(4.0f, 5.0f, 6.0f);
    //    vec3 result = create();

    //    add(result, v1, v2);
    //    std::cout << "Result of addition: " << str(result) << std::endl;

    //    normalize(result, result);
    //    std::cout << "Normalized vector: " << str(result) << std::endl;

    //    return 0;
    //}

}