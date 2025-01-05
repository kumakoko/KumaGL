#ifndef jsm_quat_h__
#define jsm_quat_h__

#include "jsm_type_defines.h"

namespace jsm
{
    // 创建一个单位四元数
    quat createQuat();

    // 设置单位四元数
    quat identityQuat(quat& out);

    // 从轴和角度设置四元数
    quat setAxisAngle(quat& out, const vec3& axis, float rad);

    // 获取四元数的轴角表示
    float getAxisAngle(vec3& out_axis, const quat& q);

    // 四元数相乘
    quat multiply(quat& out, const quat& a, const quat& b);

    // 绕X轴旋转四元数
    quat rotateX(quat& out, const quat& a, float rad);

    // 绕Y轴旋转四元数
    quat rotateY(quat& out, const quat& a, float rad);

    // 绕Z轴旋转四元数
    quat rotateZ(quat& out, const quat& a, float rad);

    // 归一化四元数
    quat normalize(quat& out, const quat& a);

    // 四元数与向量的旋转
    vec3 rotateVec3(const quat& q, const vec3& v);

    // 四元数插值（Slerp）
    quat slerp(quat& out, const quat& a, const quat& b, float t);
}

#endif // jsm_quat_h__