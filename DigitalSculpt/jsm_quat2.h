#ifndef jsm_quat2_h__
#define jsm_quat2_h__

#include "jsm_type_defines.h"

namespace jsm
{
    // Utility functions
    float dot(const quat2& q1, const quat2& q2);

    void normalize(quat2& q);

    // Identity dual quaternion
    quat2 identity();

    // Dual quaternion multiplication
    quat2 multiply(const quat2& q1, const quat2& q2);

    // Conjugate of a dual quaternion
    quat2 conjugate(const quat2& q);

    // Transform a point using a dual quaternion
    vec3 transformPoint(const quat2& dq, const vec3& point);

    // Print a dual quaternion
    void printDualQuaternion(const quat2& dq);
}

#endif // jsm_quat2_h__