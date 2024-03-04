#ifndef digital_sculpt_misc_utils_h__
#define digital_sculpt_misc_utils_h__

#include <cmath>
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>
#include <vector>
#include <cstdint>
#include <array>
#include "glm/vec3.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_inverse.hpp" // 引入逆转置功能

namespace DigitalSculpt
{
    typedef std::vector<std::uint8_t> Uint8Array;
    typedef std::vector<std::uint32_t> Uint32Array;
    typedef std::vector<std::int32_t> Int32Array;
    typedef std::vector<float>  Float32Array;
    typedef std::array<float, 6> SixElemArray;


    class Utils
    {
    public:
        static constexpr float SCALE = 100.0;
        static int TAG_FLAG = 1;
        static constexpr int SCULPT_FLAG = 1;
        static constexpr int STATE_FLAG = 1;
        static constexpr std::uint32_t TRI_INDEX = 4294967295;

        static constexpr char* dropperCursor = "url(resources/dropper.png) 5 25, auto";

        static float linearToSRGB1(float x) {
            return x < 0.0031308 ? x * 12.92 : 1.055 * std::pow(x, 1.0 / 2.4) - 0.055;
        }

        static float sRGBToLinear1(float x) {
            return x < 0.04045 ? x * (1.0 / 12.92) : std::pow((x + 0.055) * (1.0 / 1.055), 2.4);
        }

        template<typename T>
        static void extend(T& dest, const T& src) {
            for (const auto& pair : src) {
                if (dest.find(pair.first) == dest.end())
                    dest[pair.first] = pair.second;
            }
        }

        template<typename T>
        static T invert(const T& obj) {
            T inv;
            for (const auto& pair : obj)
                inv[pair.second] = pair.first;
            return inv;
        }

        template<typename T>
        static void replaceElement(std::vector<T>& array, const T& oldValue, const T& newValue) {
            auto it = std::find(array.begin(), array.end(), oldValue);
            if (it != array.end())
                *it = newValue;
        }

        template<typename T>
        static void removeElement(std::vector<T>& array, const T& remValue) {
            auto it = std::find(array.begin(), array.end(), remValue);
            if (it != array.end()) {
                *it = array.back();
                array.pop_back();
            }
        }

        template<typename T>
        static void appendArray(std::vector<T>& array1, const std::vector<T>& array2) {
            array1.insert(array1.end(), array2.begin(), array2.end());
        }

        static bool isPowerOfTwo(unsigned int x) {
            return x != 0 && (x & (x - 1)) == 0;
        }

        static unsigned int nextHighestPowerOfTwo(unsigned int x) {
            --x;
            for (unsigned int i = 1; i < 32; i <<= 1)
                x = x | x >> i;
            return x + 1;
        }

        static void tidy(std::vector<unsigned int>& array) {
            std::sort(array.begin(), array.end());
            auto it = std::unique(array.begin(), array.end());
            array.resize(std::distance(array.begin(), it));
        }

        template<typename T>
        static std::vector<T> intersectionArrays(const std::vector<T>& a, const std::vector<T>& b) {
            std::vector<T> result;
            auto it1 = a.begin();
            auto it2 = b.begin();
            while (it1 != a.end() && it2 != b.end()) {
                if (*it1 < *it2)
                    ++it1;
                else if (*it2 < *it1)
                    ++it2;
                else {
                    result.push_back(*it1);
                    ++it1;
                    ++it2;
                }
            }
            return result;
        }

        static bool littleEndian() {
            short int word = 0x0001;
            char* byte = reinterpret_cast<char*>(&word);
            return byte[0] != 0x01;
        }

        static std::vector<unsigned char> getBytes(const std::vector<unsigned char>& data, size_t offset) {
            return { data[offset], data[offset + 1], data[offset + 2], data[offset + 3] };
        }

        static unsigned int getUint32(const std::vector<unsigned char>& data, size_t offset) {
            auto b = getBytes(data, offset);
            return (b[0] << 0) | (b[1] << 8) | (b[2] << 16) | (b[3] << 24);
        }

        static float getFloat32(const std::vector<unsigned char>& data, size_t offset) {
            auto b = getBytes(data, offset);
            int sign = 1 - (2 * (b[3] >> 7));
            int exponent = (((b[3] << 1) & 0xff) | (b[2] >> 7)) - 127;
            int mantissa = ((b[2] & 0x7f) << 16) | (b[1] << 8) | b[0];

            if (exponent == 128) {
                if (mantissa != 0)
                    return NAN;
                else
                    return sign * INFINITY;
            }
            if (exponent == -127)
                return sign * mantissa * std::pow(2, -126 - 23);
            return sign * (1 + mantissa * std::pow(2, -23)) * std::pow(2, exponent);
        }

        static std::string ab2str(const std::vector<unsigned char>& buf) {
            std::string str;
            for (size_t off = 0, abLen = buf.size(); off < abLen; off += 65535) {
                size_t chunkSize = std::min<size_t>(65535, abLen - off);
                std::string subab(reinterpret_cast<const char*>(&buf[off]), chunkSize);
                str += subab;
            }
            return str;
        }

        static std::vector<unsigned char> getMemory(size_t nbBytes) {
            static std::vector<unsigned char> pool(100000);
            if (pool.size() >= nbBytes)
                return pool;
            pool.resize(nbBytes);
            return pool;
        }

        static unsigned int now() {
            return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        }

        static glm::vec3& scaleAndAdd(glm::vec3& out, const glm::vec3& a, const glm::vec3& b, float scale)
        {
            out[0] = a[0] + b[0] * scale;
            out[1] = a[1] + b[1] * scale;
            out[2] = a[2] + b[2] * scale;
            return out;
        }


        /// <summary>
        /// https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/TypedArray/subarray
        /// 仿照以上URL的js类成员方法做的实现，注意原始的JS代码的描述文档中有这么一句：
        /// 另请注意，这是在现有缓冲区上创建一个新视图；对新对象内容的更改将影响原始对象，反之亦然。
        /// 用C#代码模拟实现的，就不会出现这个问题，因为会创建一个新类
        /// </summary>
        /// <param name="self"></param>
        /// <param name="startIndex">从第几个源数组元素开始拷贝，如果从第1个源数组元素开始拷贝，则此值为0，从第2个开始拷贝，此值为1，以此类推</param>
        /// <param name="copiedElementCount"></param>
        /// <returns></returns>
        template<typename T>
        static std::vector<T> subarray(const std::vector<T>& self, int startIndex, int copiedElementCount)
        {
            std::vector<T> L;

            for (int i = startIndex; i < startIndex + copiedElementCount; ++i)
                L.emplace_back(self[i]);

            return L;
        }

        template<typename T>
        static std::size_t GetArrayElementsCount(const T* src_array)
        {
            std::size_t array_byte_count = sizeof(src_array);
            std::size_t array_elem_size = sizeof(T);
            return array_byte_count / array_elem_size;
        }


        /// <summary>
       /// 把dataSource的内容，依次拷贝到self数组中，self数组的目标索引值范围是
       /// [offset,offset+dataSource.Length]。这也就是说，self数组本身要有
       /// 足够多的空间存储拷贝数据，否则就回抛出异常，本方法是模仿javascript的TypedArray
       /// 的实现。参照网页：
       /// https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/TypedArray/set
       /// </summary>
       /// <typeparam name="T"></typeparam>
       /// <param name="self"></param>
       /// <param name="dataSource"></param>
       /// <param name="offset"></param>
       /// <returns></returns>
        template<typename T>
        static void set(std::vector<T>& self, const std::vector<T>& dataSource, std::size_t offset = 0)
        {
            std::size_t copyL = dataSource.size();

            for (std::size_t i = 0; i < copyL; ++i)
                self[i + offset] = dataSource[i];

            return self;
        }

        template<typename T>
        static void FindMaxMin(T a, T b, T c, T& max, T& min)
        {
            max = a;
            min = a;

            // 检查第二个数
            if (b > max)
                max = b;

            if (b < min)
                min = b;

            // 检查第三个数
            if (c > max)
                max = c;

            if (c < min)
                min = c;
        }

        /*webgl的glMatrix库中的mat3.normalFromMat4函数，对应于glm库中的什么函数
        
        ChatGPT:
        在WebGL中使用的glMatrix库和C++中使用的GLM（OpenGL Mathematics）库都提供了
        丰富的数学运算功能，特别是在图形编程中经常用到的矩阵和向量运算。glMatrix库中的
        mat3.normalFromMat4函数用于从一个4x4的模型视图矩阵中提取出一个用于变换法线的
        3x3矩阵。这个过程通常涉及到计算原4x4矩阵的逆转置矩阵，但仅针对左上角的3x3部分，
        因为这部分负责旋转和缩放，这对法线变换是必要的。

        在GLM库中，进行相似操作的函数是glm::inverseTranspose。这个函数可以接受一个4x4
        的矩阵，并返回其逆转置矩阵。但是，为了获得与glMatrix中mat3.normalFromMat4相同
        的结果，你可能需要手动提取返回矩阵的左上角3x3部分。这是因为GLM处理的是完整的4x4矩
        阵，而不是自动抽取3x3部分。

        具体而言，你可以先使用glm::inverseTranspose对4x4矩阵进行逆转置，然后提取3x3部分
        以应用于法线变换。代码示例如下：
        
        #include <glm/glm.hpp>
        #include <glm/gtc/matrix_inverse.hpp> // 引入逆转置功能

        glm::mat4 modelViewMatrix = ...; // 你的4x4模型视图矩阵
        glm::mat4 invTransModelViewMatrix = glm::inverseTranspose(modelViewMatrix);

        // 现在，如果你需要3x3的法线变换矩阵，可以这样获取：
        glm::mat3 normalMatrix = glm::mat3(invTransModelViewMatrix);
        */
        static glm::mat3& normalFromMat4(glm::mat3& m3, const glm::mat4& m4)
        {
            glm::mat4 invTransModelViewMatrix = glm::inverseTranspose(m4);
            m3 = glm::mat3(invTransModelViewMatrix);
            return m3;
        }





    };
}

#endif // digital_sculpt_misc_utils_h__