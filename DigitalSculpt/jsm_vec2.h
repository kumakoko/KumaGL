#ifndef jsm_vec2_h__
#define jsm_vec2_h__

namespace jsm
{
    // ���� vec2 ����
    using vec2 = std::vector<float>;
    using ReadonlyVec2 = const vec2&;

    // ����һ���µ� vec2
    vec2 create();

    // ��¡һ�����е� vec2
    vec2 clone(ReadonlyVec2 a);

    // ���ݸ���ֵ���� vec2
    vec2 fromValues(float x, float y);

    // ���� vec2 ��ֵ
    vec2& copy(vec2& out, ReadonlyVec2 a);

    // ���� vec2 ��ֵ
    vec2& set(vec2& out, float x, float y);

    // vec2 �ӷ�
    vec2& add(vec2& out, ReadonlyVec2 a, ReadonlyVec2 b);

    // vec2 ����
    vec2& subtract(vec2& out, ReadonlyVec2 a, ReadonlyVec2 b);

    // vec2 �˷�
    vec2& multiply(vec2& out, ReadonlyVec2 a, ReadonlyVec2 b);

    // vec2 ����
    vec2& divide(vec2& out, ReadonlyVec2 a, ReadonlyVec2 b);

    // ����ȡ��
    vec2& ceil(vec2& out, ReadonlyVec2 a);

    // ����ȡ��
    vec2& floor(vec2& out, ReadonlyVec2 a);

    // �������� vec2 ����Сֵ
    vec2& min(vec2& out, ReadonlyVec2 a, ReadonlyVec2 b);

    // �������� vec2 �����ֵ
    vec2& max(vec2& out, ReadonlyVec2 a, ReadonlyVec2 b);

    // ���� vec2
    vec2& scale(vec2& out, ReadonlyVec2 a, float b);

    // ���� vec2 ��ŷ����ó���
    float length(ReadonlyVec2 a);

    // ��׼�� vec2
    vec2& normalize(vec2& out, ReadonlyVec2 a);

    // �������� vec2 �ĵ��
    float dot(ReadonlyVec2 a, ReadonlyVec2 b);

    // ���� vec2 ����ת
    vec2& rotate(vec2& out, ReadonlyVec2 a, ReadonlyVec2 b, float rad);

    // ��ӡ vec2 Ϊ�ַ���
    std::string str(ReadonlyVec2 a);

    // ������� vec2 �Ƿ���ȫ���
    bool exactEquals(ReadonlyVec2 a, ReadonlyVec2 b);

    // ������� vec2 �Ƿ�������
    bool equals(ReadonlyVec2 a, ReadonlyVec2 b);
}

#endif // jsm_vec2_h__
