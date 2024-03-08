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
        static int TAG_FLAG;
        static int SCULPT_FLAG;
        static constexpr int STATE_FLAG = 1;
        static constexpr std::uint32_t TRI_INDEX = 4294967295;
        static constexpr float SQRT1_2 = 0.70710678f;
        static constexpr char* dropperCursor = "url(resources/dropper.png) 5 25, auto";

        inline static float linearToSRGB1(float x)
        {
            return x < 0.0031308f ? x * 12.92f : 1.055f * std::pow(x, 1.0f / 2.4f) - 0.055f;
        }

        inline static float sRGBToLinear1(float x)
        {
            return x < 0.04045f ? x * (1.0f / 12.92f) : std::pow((x + 0.055f) * (1.0f / 1.055f), 2.4f);
        }

        template<typename T>
        static void extend(T& dest, const T& src)
        {
            for (const auto& pair : src) {
                if (dest.find(pair.first) == dest.end())
                    dest[pair.first] = pair.second;
            }
        }

        template<typename T>
        static T invert(const T& obj)
        {
            T inv;
            for (const auto& pair : obj)
                inv[pair.second] = pair.first;
            return inv;
        }

        template<typename T>
        static void replaceElement(std::vector<T>& array, const T& oldValue, const T& newValue)
        {
            auto it = std::find(array.begin(), array.end(), oldValue);
            if (it != array.end())
                *it = newValue;
        }

        template<typename T>
        static void removeElement(std::vector<T>& array, const T& remValue)
        {
            auto it = std::find(array.begin(), array.end(), remValue);
            if (it != array.end()) {
                *it = array.back();
                array.pop_back();
            }
        }

        template<typename T>
        inline static void appendArray(std::vector<T>& array1, const std::vector<T>& array2)
        {
            array1.insert(array1.end(), array2.begin(), array2.end());
        }

        inline static bool isPowerOfTwo(unsigned int x)
        {
            return x != 0 && (x & (x - 1)) == 0;
        }

        inline static unsigned int nextHighestPowerOfTwo(unsigned int x)
        {
            --x;
            for (unsigned int i = 1; i < 32; i <<= 1)
                x = x | x >> i;
            return x + 1;
        }

        static void tidy(std::vector<unsigned int>& array);

        template<typename T>
        static std::vector<T> intersectionArrays(const std::vector<T>& a, const std::vector<T>& b)
        {
            std::vector<T> result;
            auto it1 = a.begin();
            auto it2 = b.begin();

            while (it1 != a.end() && it2 != b.end())
            {
                if (*it1 < *it2)
                    ++it1;
                else if (*it2 < *it1)
                    ++it2;
                else
                {
                    result.push_back(*it1);
                    ++it1;
                    ++it2;
                }
            }
            return result;
        }

        inline static bool littleEndian()
        {
            short int word = 0x0001;
            char* byte = reinterpret_cast<char*>(&word);
            return byte[0] != 0x01;
        }

        inline static std::vector<unsigned char> getBytes(const std::vector<unsigned char>& data, size_t offset)
        {
            return { data[offset], data[offset + 1], data[offset + 2], data[offset + 3] };
        }

        static unsigned int getUint32(const std::vector<unsigned char>& data, size_t offset);

        static float getFloat32(const std::vector<unsigned char>& data, size_t offset);

        static std::string ab2str(const std::vector<unsigned char>& buf);

        static std::vector<unsigned char> getMemory(size_t nbBytes);

        inline static unsigned int now()
        {
            return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        }

        /******************************************************************************************************************
         * Desc: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/TypedArray/subarray
         *       仿照以上URL的js类成员方法做的实现，注意原始的JS代码的描述文档中有这么一句：
         *       另请注意，这是在现有缓冲区上创建一个新视图；对新对象内容的更改将影响原始对象，反之亦然。
         *       用C#代码模拟实现的，就不会出现这个问题，因为会创建一个新类
         * Method:    subarray
         * Returns:
         * Parameter: const std::vector<T> & self
         * Parameter: int startIndex
         * Parameter: int copiedElementCount
         ****************************************************************************************************************/
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

        /******************************************************************************************************************
         * Desc: 把dataSource的内容，依次拷贝到self数组中，self数组的目标索引值范围是[offset,offset+dataSource.Length]。这也就是说，
                 self数组本身要有足够多的空间存储拷贝数据，否则就回抛出异常，本方法是模仿javascript的TypedArray的实现。参照网页
                 https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/TypedArray/set
         * Method:    set
         * Returns:   void
         * Parameter: std::vector<T> & self
         * Parameter: const std::vector<T> & dataSource
         * Parameter: std::size_t offset
         ****************************************************************************************************************/
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

        static float Utils::easeOutQuart(float r);
    };
}

#endif // digital_sculpt_misc_utils_h__