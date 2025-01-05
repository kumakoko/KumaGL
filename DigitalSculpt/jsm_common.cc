#include "../klib/kgl_lib_pch.h"
#include "jsm_common.h"

namespace jsm
{
    double RandomFunc()
    {
        return static_cast<double>(rand()) / RAND_MAX; 
    }

    std::string ANGLE_ORDER = "zyx";

    const double EPSILON = 0.000001;

    // Helper for Degree to Radian Conversion
    const double DEGREE = 3.14159265358979323846 / 180.0;

    int round(double a)
    {
        if (a >= 0) {
            return std::round(a);
        }
        return (std::fmod(a, 0.5) == 0) ? std::floor(a) : std::round(a);
    }

    /**
     * Sets the type of array used when creating new vectors and matrices
     *
     * @param type Placeholder for C++ (no direct type switching as in JavaScript)
     */
    void setMatrixArrayType()
    {
        // In C++, we typically define the type statically, so this is a no-op.
        // For runtime behavior, consider using std::variant or similar.
    }

    /**
     * Convert Degree To Radian
     *
     * @param a Angle in Degrees
     * @return Angle in Radians
     */
    double toRadian(double a)
    {
        return a * DEGREE;
    }

    /**
     * Tests whether or not the arguments have approximately the same value,
     * within an absolute or relative tolerance of EPSILON.
     *
     * @param a The first number to test.
     * @param b The second number to test.
     * @return True if the numbers are approximately equal, false otherwise.
     */
    bool equals(double a, double b)
    {
        return std::abs(a - b) <= EPSILON * std::max({ 1.0, std::abs(a), std::abs(b) });
    }
}