#ifndef jsm_common_h__
#define jsm_common_h__

namespace jsm
{
    // Configuration Constants
    extern const double EPSILON;

    extern double RandomFunc();

    extern std::string ANGLE_ORDER;

    // Helper for Degree to Radian Conversion
    extern const double DEGREE;

    /**
     * Symmetric round
     * Detailed background: https://www.npmjs.com/package/round-half-up-symmetric#user-content-detailed-background
     *
     * @param a Value to round
     * @return Rounded value
     */
    int round(double a);

    /**
     * Sets the type of array used when creating new vectors and matrices
     *
     * @param type Placeholder for C++ (no direct type switching as in JavaScript)
     */
    void setMatrixArrayType();

    /**
     * Convert Degree To Radian
     *
     * @param a Angle in Degrees
     * @return Angle in Radians
     */
    double toRadian(double a);

    /**
     * Tests whether or not the arguments have approximately the same value,
     * within an absolute or relative tolerance of EPSILON.
     *
     * @param a The first number to test.
     * @param b The second number to test.
     * @return True if the numbers are approximately equal, false otherwise.
     */
    bool equals(double a, double b);

    // 变长模板参数，单个参数的版本
    template <typename T>
    T max(T a) 
    {
        return a; // Base case: single argument
    }

    // 可变参数个数的max方法
    template <typename T, typename... Args>
    T max(T a, Args... args) 
    {
        return std::max(a, max(args...)); // 递归比较
    }

    // 基于初始化列表的版本
    template <typename T>
    T max(std::initializer_list<T> values) 
    {
        return *std::max_element(values.begin(), values.end());
    }
}

#endif // jsm_common_h__