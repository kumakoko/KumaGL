#ifndef jsm_vec2_h__
#define jsm_vec2_h__

namespace jsm
{
    // 定义 vec2 类型
    using vec2 = std::vector<float>;
    using ReadonlyVec2 = const vec2&;

    // 创建一个新的 vec2
    vec2 create();

    // 克隆一个现有的 vec2
    vec2 clone(ReadonlyVec2 a);

    // 根据给定值创建 vec2
    vec2 fromValues(float x, float y);

    // 拷贝 vec2 的值
    vec2& copy(vec2& out, ReadonlyVec2 a);

    // 设置 vec2 的值
    vec2& set(vec2& out, float x, float y);

    // vec2 加法
    vec2& add(vec2& out, ReadonlyVec2 a, ReadonlyVec2 b);

    // vec2 减法
    vec2& subtract(vec2& out, ReadonlyVec2 a, ReadonlyVec2 b);

    // vec2 乘法
    vec2& multiply(vec2& out, ReadonlyVec2 a, ReadonlyVec2 b);

    // vec2 除法
    vec2& divide(vec2& out, ReadonlyVec2 a, ReadonlyVec2 b);

    // 向上取整
    vec2& ceil(vec2& out, ReadonlyVec2 a);

    // 向下取整
    vec2& floor(vec2& out, ReadonlyVec2 a);

    // 返回两个 vec2 的最小值
    vec2& min(vec2& out, ReadonlyVec2 a, ReadonlyVec2 b);

    // 返回两个 vec2 的最大值
    vec2& max(vec2& out, ReadonlyVec2 a, ReadonlyVec2 b);

    // 缩放 vec2
    vec2& scale(vec2& out, ReadonlyVec2 a, float b);

    // 计算 vec2 的欧几里得长度
    float length(ReadonlyVec2 a);

    // 标准化 vec2
    vec2& normalize(vec2& out, ReadonlyVec2 a);

    // 计算两个 vec2 的点积
    float dot(ReadonlyVec2 a, ReadonlyVec2 b);

    // 计算 vec2 的旋转
    vec2& rotate(vec2& out, ReadonlyVec2 a, ReadonlyVec2 b, float rad);

    // 打印 vec2 为字符串
    std::string str(ReadonlyVec2 a);

    // 检查两个 vec2 是否完全相等
    bool exactEquals(ReadonlyVec2 a, ReadonlyVec2 b);

    // 检查两个 vec2 是否近似相等
    bool equals(ReadonlyVec2 a, ReadonlyVec2 b);
}

#endif // jsm_vec2_h__
