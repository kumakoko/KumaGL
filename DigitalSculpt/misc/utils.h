#ifndef digital_sculpt_misc_utils_h__
#define digital_sculpt_misc_utils_h__

#include <cmath>
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>
#include <vector>
#include <cstdint>
#include "glm/vec3.hpp"

namespace DigitalSculpt
{
    typedef std::vector<std::uint8_t> Uint8Array;
    typedef std::vector<std::uint32_t> Uint32Array;
    typedef std::vector<std::int32_t> Int32Array;
    typedef std::vector<float>  Float32Array;
    

    class Utils
    {
    public:
        static constexpr float SCALE = 100.0;
        static constexpr int TAG_FLAG = 1;
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
        static T* subarray(const std::vector<T>& self, int startIndex, int copiedElementCount)
        {
            T* L = new T[copiedElementCount];

            for (int i = startIndex; i < startIndex + copiedElementCount; ++i)
                L[i] = self[i];

            return L;
        }











    };
}

#endif // digital_sculpt_misc_utils_h__