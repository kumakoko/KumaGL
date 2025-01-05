#include "../klib/kgl_lib_pch.h"
#include "jsm_quat.h"

namespace jsm
{
    // 创建一个单位四元数
    quat createQuat() 
    {
        quat q = { 0.0f, 0.0f, 0.0f, 1.0f };
        return q;
    }

    // 设置单位四元数
    quat identityQuat(quat& out) 
    {
        out = { 0.0f, 0.0f, 0.0f, 1.0f };
        return out;
    }

    // 从轴和角度设置四元数
    quat setAxisAngle(quat& out, const vec3& axis, float rad)
    {
        rad *= 0.5f;
        float s = std::sin(rad);
        out[0] = s * axis[0];
        out[1] = s * axis[1];
        out[2] = s * axis[2];
        out[3] = cos(rad);
        return out;
    }

    // 获取四元数的轴角表示
    float getAxisAngle(vec3& out_axis, const quat& q) 
    {
        float rad = std::acos(q[3]) * 2.0f;
        float s = std::sin(rad / 2.0f);
        
        if (s > 1e-5f)
        {
            out_axis[0] = q[0] / s;
            out_axis[1] = q[1] / s;
            out_axis[2] = q[2] / s;
        }
        else
        {
            out_axis = { 1.0f, 0.0f, 0.0f };
        }

        return rad;
    }

    // 四元数相乘
    quat multiply(quat& out, const quat& a, const quat& b)
    {
        float ax = a[0], ay = a[1], az = a[2], aw = a[3];
        float bx = b[0], by = b[1], bz = b[2], bw = b[3];

        out[0] = ax * bw + aw * bx + ay * bz - az * by;
        out[1] = ay * bw + aw * by + az * bx - ax * bz;
        out[2] = az * bw + aw * bz + ax * by - ay * bx;
        out[3] = aw * bw - ax * bx - ay * by - az * bz;
        return out;
    }

    // 绕X轴旋转四元数
    quat rotateX(quat& out, const quat& a, float rad) 
    {
        rad *= 0.5f;
        float bx = sin(rad), bw = cos(rad);

        out[0] = a[0] * bw + a[3] * bx;
        out[1] = a[1] * bw + a[2] * bx;
        out[2] = a[2] * bw - a[1] * bx;
        out[3] = a[3] * bw - a[0] * bx;
        return out;
    }

    // 绕Y轴旋转四元数
    quat rotateY(quat& out, const quat& a, float rad) 
    {
        rad *= 0.5f;
        float by = sin(rad), bw = cos(rad);

        out[0] = a[0] * bw - a[2] * by;
        out[1] = a[1] * bw + a[3] * by;
        out[2] = a[2] * bw + a[0] * by;
        out[3] = a[3] * bw - a[1] * by;
        return out;
    }

    // 绕Z轴旋转四元数
    quat rotateZ(quat& out, const quat& a, float rad)
    {
        rad *= 0.5f;
        float bz = sin(rad), bw = cos(rad);

        out[0] = a[0] * bw + a[1] * bz;
        out[1] = a[1] * bw - a[0] * bz;
        out[2] = a[2] * bw + a[3] * bz;
        out[3] = a[3] * bw - a[2] * bz;
        return out;
    }

    // 归一化四元数
    quat normalize(quat& out, const quat& a) 
    {
        float len = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2] + a[3] * a[3]);

        if (len > 0.0f) 
        {
            len = 1.0f / len;
            out[0] = a[0] * len;
            out[1] = a[1] * len;
            out[2] = a[2] * len;
            out[3] = a[3] * len;
        }

        return out;
    }

    // 四元数与向量的旋转
    vec3 rotateVec3(const quat& q, const vec3& v) 
    {
        quat q_conj = { -q[0], -q[1], -q[2], q[3] };
        quat vec_as_quat = { v[0], v[1], v[2], 0.0f };
        quat tmp, rotated;

        multiply(tmp, q, vec_as_quat);
        multiply(rotated, tmp, q_conj);

        return { rotated[0], rotated[1], rotated[2] };
    }

    // 四元数插值（Slerp）
    quat slerp(quat& out, const quat& a, const quat& b, float t) 
    {
        float cosTheta = a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
        quat bAdjusted = b;

        if (cosTheta < 0.0f)
        {
            cosTheta = -cosTheta;
            bAdjusted[0] = -b[0];
            bAdjusted[1] = -b[1];
            bAdjusted[2] = -b[2];
            bAdjusted[3] = -b[3];
        }

        if (cosTheta > 0.9995f) 
        {
            for (int i = 0; i < 4; ++i)
                out[i] = a[i] + t * (bAdjusted[i] - a[i]);

            normalize(out, out);
            return out;
        }

        float theta0 = acos(cosTheta);
        float theta = theta0 * t;

        float sinTheta = sin(theta);
        float sinTheta0 = sin(theta0);

        float s0 = cos(theta) - cosTheta * sinTheta / sinTheta0;
        float s1 = sinTheta / sinTheta0;

        for (int i = 0; i < 4; ++i)
            out[i] = s0 * a[i] + s1 * bAdjusted[i];

        return out;
    }
}