#include "../klib/kgl_lib_pch.h"
#include "jsm_common.h"
#include "jsm_vec2.h"

namespace jsm
{
    // ���峣��
    //const float EPSILON = 0.000001f;
    //const float DEGREE = 3.14159265358979323846f / 180.0f;

    // ����һ���µ� vec2
    vec2 create() 
    {
        return vec2(2, 0.0f); // ��ʼ��Ϊ������
    }

    // ��¡һ�����е� vec2
    vec2 clone(ReadonlyVec2 a) 
    {
        return vec2{ a[0], a[1] };
    }

    // ���ݸ���ֵ���� vec2
    vec2 fromValues(float x, float y)
    {
        return vec2{ x, y };
    }

    // ���� vec2 ��ֵ
    vec2& copy(vec2& out, ReadonlyVec2 a) 
    {
        out[0] = a[0];
        out[1] = a[1];
        return out;
    }

    // ���� vec2 ��ֵ
    vec2& set(vec2& out, float x, float y) 
    {
        out[0] = x;
        out[1] = y;
        return out;
    }

    // vec2 �ӷ�
    vec2& add(vec2& out, ReadonlyVec2 a, ReadonlyVec2 b) 
    {
        out[0] = a[0] + b[0];
        out[1] = a[1] + b[1];
        return out;
    }

    // vec2 ����
    vec2& subtract(vec2& out, ReadonlyVec2 a, ReadonlyVec2 b)
    {
        out[0] = a[0] - b[0];
        out[1] = a[1] - b[1];
        return out;
    }

    // vec2 �˷�
    vec2& multiply(vec2& out, ReadonlyVec2 a, ReadonlyVec2 b)
    {
        out[0] = a[0] * b[0];
        out[1] = a[1] * b[1];
        return out;
    }

    // vec2 ����
    vec2& divide(vec2& out, ReadonlyVec2 a, ReadonlyVec2 b) {
        out[0] = a[0] / b[0];
        out[1] = a[1] / b[1];
        return out;
    }

    // ����ȡ��
    vec2& ceil(vec2& out, ReadonlyVec2 a) 
    {
        out[0] = std::ceil(a[0]);
        out[1] = std::ceil(a[1]);
        return out;
    }

    // ����ȡ��
    vec2& floor(vec2& out, ReadonlyVec2 a) 
    {
        out[0] = std::floor(a[0]);
        out[1] = std::floor(a[1]);
        return out;
    }

    // �������� vec2 ����Сֵ
    vec2& min(vec2& out, ReadonlyVec2 a, ReadonlyVec2 b) 
    {
        out[0] = std::min(a[0], b[0]);
        out[1] = std::min(a[1], b[1]);
        return out;
    }

    // �������� vec2 �����ֵ
    vec2& max(vec2& out, ReadonlyVec2 a, ReadonlyVec2 b)
    {
        out[0] = std::max(a[0], b[0]);
        out[1] = std::max(a[1], b[1]);
        return out;
    }

    // ���� vec2
    vec2& scale(vec2& out, ReadonlyVec2 a, float b)
    {
        out[0] = a[0] * b;
        out[1] = a[1] * b;
        return out;
    }

    // ���� vec2 ��ŷ����ó���
    float length(ReadonlyVec2 a) 
    {
        return std::sqrt(a[0] * a[0] + a[1] * a[1]);
    }

    // ��׼�� vec2
    vec2& normalize(vec2& out, ReadonlyVec2 a)
    {
        float len = length(a);
        if (len > 0) {
            out[0] = a[0] / len;
            out[1] = a[1] / len;
        }
        return out;
    }

    // �������� vec2 �ĵ��
    float dot(ReadonlyVec2 a, ReadonlyVec2 b) 
    {
        return a[0] * b[0] + a[1] * b[1];
    }

    // ���� vec2 ����ת
    vec2& rotate(vec2& out, ReadonlyVec2 a, ReadonlyVec2 b, float rad)
    {
        float p0 = a[0] - b[0];
        float p1 = a[1] - b[1];
        float sinC = std::sin(rad);
        float cosC = std::cos(rad);

        out[0] = p0 * cosC - p1 * sinC + b[0];
        out[1] = p0 * sinC + p1 * cosC + b[1];
        return out;
    }

    // ��ӡ vec2 Ϊ�ַ���
    std::string str(ReadonlyVec2 a) 
    {
        return "vec2(" + std::to_string(a[0]) + ", " + std::to_string(a[1]) + ")";
    }

    // ������� vec2 �Ƿ���ȫ���
    bool exactEquals(ReadonlyVec2 a, ReadonlyVec2 b)
    {
        return a[0] == b[0] && a[1] == b[1];
    }

    // ������� vec2 �Ƿ�������
    bool equals(ReadonlyVec2 a, ReadonlyVec2 b)
    {
        if (a.size() != 2 || b.size() != 2) 
        {
            throw std::invalid_argument("Vectors must have exactly two elements.");
        }

        return std::abs(a[0] - b[0]) <= EPSILON * max(1.0f, std::abs(a[0]), std::abs(b[0])) &&
            std::abs(a[1] - b[1]) <= EPSILON * max(1.0f, std::abs(a[1]), std::abs(b[1]));
    }

    //int main() {
    //    // ʾ��ʹ��
    //    vec2 v1 = fromValues(1.0f, 2.0f);
    //    vec2 v2 = fromValues(3.0f, 4.0f);
    //    vec2 result = create();

    //    add(result, v1, v2);
    //    std::cout << "Result of addition: " << str(result) << std::endl;

    //    normalize(result, result);
    //    std::cout << "Normalized vector: " << str(result) << std::endl;

    //    return 0;
    //}
}