#include "../klib/kgl_lib_pch.h"
#include "jsm_common.h"
#include "jsm_mat2.h"

namespace jsm
{
    // Utility functions
    float dot(const quat2& q1, const quat2& q2)
    {
        float result = 0.0f;
        for (int i = 0; i < 4; ++i) {
            result += q1[i] * q2[i];
        }
        return result;
    }

    void normalize(quat2& q)
    {
        float norm = std::sqrt(dot(q, q));

        for (float& val : q)
        {
            val /= norm;
        }
    }

    // Identity dual quaternion
    quat2 identity()
    {
        return { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    }

    // Dual quaternion multiplication
    quat2 multiply(const quat2& q1, const quat2& q2)
    {
        quat2 result;

        // Real part
        result[0] = q1[0] * q2[0] - q1[1] * q2[1] - q1[2] * q2[2] - q1[3] * q2[3];
        result[1] = q1[0] * q2[1] + q1[1] * q2[0] + q1[2] * q2[3] - q1[3] * q2[2];
        result[2] = q1[0] * q2[2] - q1[1] * q2[3] + q1[2] * q2[0] + q1[3] * q2[1];
        result[3] = q1[0] * q2[3] + q1[1] * q2[2] - q1[2] * q2[1] + q1[3] * q2[0];

        // Dual part
        result[4] = q1[4] * q2[0] + q1[0] * q2[4] - q1[5] * q2[1] - q1[6] * q2[2] - q1[7] * q2[3];
        result[5] = q1[4] * q2[1] + q1[5] * q2[0] + q1[0] * q2[5] + q1[6] * q2[3] - q1[7] * q2[2];
        result[6] = q1[4] * q2[2] - q1[5] * q2[3] + q1[6] * q2[0] + q1[0] * q2[6] + q1[7] * q2[1];
        result[7] = q1[4] * q2[3] + q1[5] * q2[2] - q1[6] * q2[1] + q1[7] * q2[0] + q1[0] * q2[7];

        return result;
    }

    // Conjugate of a dual quaternion
    quat2 conjugate(const quat2& q)
    {
        return { q[0], -q[1], -q[2], -q[3], -q[4], -q[5], -q[6], -q[7] };
    }

    // Transform a point using a dual quaternion
    vec3 transformPoint(const quat2& dq, const vec3& point)
    {
        quat2 pointDq = { 0.0f, point[0], point[1], point[2], 0.0f, 0.0f, 0.0f, 0.0f };
        quat2 dqConjugate = conjugate(dq);
        quat2 transformedDq = multiply(multiply(dq, pointDq), dqConjugate);
        return { transformedDq[1], transformedDq[2], transformedDq[3] };
    }

    // Print a dual quaternion
    void printDualQuaternion(const quat2& dq)
    {
        //std::cout << "Real: (" << dq[0] << ", " << dq[1] << ", " << dq[2] << ", " << dq[3] << ")\n";
        //std::cout << "Dual: (" << dq[4] << ", " << dq[5] << ", " << dq[6] << ", " << dq[7] << ")\n";
    }

    //int main() {
    //    // Example usage
    //    quat2 dq1 = identity();
    //    quat2 dq2 = { 1.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 0.5f };

    //    quat2 result = multiply(dq1, dq2);

    //    std::cout << "Result of multiplication:\n";
    //    printDualQuaternion(result);

    //    std::array<float, 3> point = { 1.0f, 2.0f, 3.0f };
    //    auto transformedPoint = transformPoint(result, point);

    //    std::cout << "Transformed point: (" << transformedPoint[0] << ", " << transformedPoint[1] << ", " << transformedPoint[2] << ")\n";

    //    return 0;
    //}

}