#ifndef utils_h__
#define utils_h__

#include "../klib/kgl_lib_pch.h"

namespace DigitalSculpt
{
    typedef std::vector<std::uint8_t> Uint8Array;
    typedef std::vector<std::uint32_t> Uint32Array;
    typedef std::vector<std::int32_t> Int32Array;
    typedef std::vector<std::int8_t> Int8Array;
    typedef std::vector<float>  Float32Array;
    typedef std::array<float, 6> SixElemArray;

    typedef std::vector<glm::vec3> Vec3Array;

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

        /******************************************************************************************************************
         * Desc:
         * Method:    linearToSRGB1
         * Returns:   float
         * Parameter: float x
         ****************************************************************************************************************/
        inline static float linearToSRGB1(float x)
        {
            return x < 0.0031308f ? x * 12.92f : 1.055f * std::pow(x, 1.0f / 2.4f) - 0.055f;
        }

        /******************************************************************************************************************
         * Desc:
         * Method:    sRGBToLinear1
         * Returns:   float
         * Parameter: float x
         ****************************************************************************************************************/
        inline static float sRGBToLinear1(float x)
        {
            return x < 0.04045f ? x * (1.0f / 12.92f) : std::pow((x + 0.055f) * (1.0f / 1.055f), 2.4f);
        }

        /******************************************************************************************************************
         * Desc:
         * Method:    extend
         * Returns:   void
         * Parameter: T & dest
         * Parameter: const T & src
         ****************************************************************************************************************/
        template<typename T>
        static void extend(T& dest, const T& src)
        {
            for (const auto& pair : src) {
                if (dest.find(pair.first) == dest.end())
                    dest[pair.first] = pair.second;
            }
        }

        /******************************************************************************************************************
         * Desc:
         * Method:    invert
         * Returns:   boost::T
         * Parameter: const T & obj
         ****************************************************************************************************************/
        template<typename T>
        static T invert(const T& obj)
        {
            T inv;
            for (const auto& pair : obj)
                inv[pair.second] = pair.first;
            return inv;
        }

        /******************************************************************************************************************
         * Desc:
         * Method:    replaceElement
         * Returns:   void
         * Parameter: std::vector<T> & array
         * Parameter: const T & oldValue
         * Parameter: const T & newValue
         ****************************************************************************************************************/
        template<typename T>
        static void replaceElement(std::vector<T>& array, const T& oldValue, const T& newValue)
        {
            auto it = std::find(array.begin(), array.end(), oldValue);
            if (it != array.end())
                *it = newValue;
        }

        /******************************************************************************************************************
         * Desc:
         * Method:    removeElement
         * Returns:   void
         * Parameter: std::vector<T> & array
         * Parameter: const T & remValue
         ****************************************************************************************************************/
        template<typename T>
        static void removeElement(std::vector<T>& array, const T& remValue)
        {
            auto it = std::find(array.begin(), array.end(), remValue);
            if (it != array.end()) {
                *it = array.back();
                array.pop_back();
            }
        }

        /******************************************************************************************************************
         * Desc:
         * Method:    appendArray
         * Returns:   void
         * Parameter: std::vector<T> & array1
         * Parameter: const std::vector<T> & array2
         ****************************************************************************************************************/
        template<typename T>
        inline static void appendArray(std::vector<T>& array1, const std::vector<T>& array2)
        {
            array1.insert(array1.end(), array2.begin(), array2.end());
        }

        /******************************************************************************************************************
         * Desc:
         * Method:    isPowerOfTwo
         * Returns:   bool
         * Parameter: unsigned int x
         ****************************************************************************************************************/
        inline static bool isPowerOfTwo(unsigned int x)
        {
            return x != 0 && (x & (x - 1)) == 0;
        }

        /******************************************************************************************************************
         * Desc:
         * Method:    nextHighestPowerOfTwo
         * Returns:   unsigned int
         * Parameter: unsigned int x
         ****************************************************************************************************************/
        inline static unsigned int nextHighestPowerOfTwo(unsigned int x)
        {
            --x;
            for (unsigned int i = 1; i < 32; i <<= 1)
                x = x | x >> i;
            return x + 1;
        }

        /******************************************************************************************************************
         * Desc:
         * Method:    tidy
         * Returns:   void
         * Parameter: std::vector<unsigned int> & array
         ****************************************************************************************************************/
        static void tidy(std::vector<unsigned int>& array);

        /******************************************************************************************************************
         * Desc:
         * Method:    intersectionArrays
         * Returns:
         * Parameter: const std::vector<T> & a
         * Parameter: const std::vector<T> & b
         ****************************************************************************************************************/
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

        /******************************************************************************************************************
         * Desc:
         * Method:    littleEndian
         * Returns:   bool
         ****************************************************************************************************************/
        inline static bool littleEndian()
        {
            short int word = 0x0001;
            char* byte = reinterpret_cast<char*>(&word);
            return byte[0] != 0x01;
        }

        /******************************************************************************************************************
         * Desc:
         * Method:    getBytes
         * Returns:   std::vector<unsigned char>
         * Parameter: const std::vector<unsigned char> & data
         * Parameter: size_t offset
         ****************************************************************************************************************/
        inline static std::vector<unsigned char> getBytes(const std::vector<unsigned char>& data, size_t offset)
        {
            return { data[offset], data[offset + 1], data[offset + 2], data[offset + 3] };
        }

        /******************************************************************************************************************
         * Desc:
         * Method:    getUint32
         * Returns:   unsigned int
         * Parameter: const std::vector<unsigned char> & data
         * Parameter: size_t offset
         ****************************************************************************************************************/
        static unsigned int getUint32(const std::vector<unsigned char>& data, size_t offset);

        /******************************************************************************************************************
         * Desc:
         * Method:    getFloat32
         * Returns:   float
         * Parameter: const std::vector<unsigned char> & data
         * Parameter: size_t offset
         ****************************************************************************************************************/
        static float getFloat32(const std::vector<unsigned char>& data, size_t offset);

        /******************************************************************************************************************
         * Desc:
         * Method:    ab2str
         * Returns:   std::string
         * Parameter: const std::vector<unsigned char> & buf
         ****************************************************************************************************************/
        static std::string ab2str(const std::vector<unsigned char>& buf);

        /******************************************************************************************************************
         * Desc:
         * Method:    getMemory
         * Returns:   std::vector<unsigned char>
         * Parameter: size_t nbBytes
         ****************************************************************************************************************/
        static std::vector<unsigned char> getMemory(size_t nbBytes);

        /******************************************************************************************************************
         * Desc:
         * Method:    now
         * Returns:   unsigned int
         ****************************************************************************************************************/
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

        /******************************************************************************************************************
         * Desc:
         * Method:    GetArrayElementsCount
         * Returns:   std::size_t
         * Parameter: const T * src_array
         ****************************************************************************************************************/
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
       @name: DigitalSculpt::Utils::set
       @return: void
       @param: std::vector<T> & self
       @param: const std::vector<T> & dataSource
       @param: std::size_t offset
       @param: std::size_t src_start_offset
       @param: std::size_t src_copy_count
       ****************************************************************************************************************/
        template<typename T>
        static void set(std::vector<T>& self, const std::vector<T>& dataSource, std::size_t offset = 0, std::size_t src_start_offset = 0, std::size_t src_copy_count = 0)
        {
            std::size_t copyL = src_copy_count == 0 ? dataSource.size() : src_copy_count;

            for (std::size_t i = 0; i < copyL; ++i)
                self[i + offset] = dataSource[i + src_start_offset];

            return self;
        }

        /******************************************************************************************************************
         * Desc:
         * Method:    FindMaxMin
         * Returns:   void
         * Parameter: T a
         * Parameter: T b
         * Parameter: T c
         * Parameter: T & max
         * Parameter: T & min
         ****************************************************************************************************************/
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


        /******************************************************************************************************************
         * Desc:
         * Method:    slice
         * Returns:
         * Parameter: const std::vector<T> & vec
         * Parameter: int start 切片对应于源数组的起始索引
         * Parameter: int end 切片对应于源数组的结束索引
         ****************************************************************************************************************/
        template <typename T>
        std::vector<T> slice(const std::vector<T>& vec, int start, int end)
        {
            // 处理负索引的情况
            int vecSize = static_cast<int>(vec.size());
            if (start < 0) start = vecSize + start;
            if (end < 0) end = vecSize + end;

            // 保证起始和结束位置的有效性
            start = std::max(start, 0);
            end = std::min(end, vecSize);

            // 如果起始位置大于结束位置或数组长度，返回一个空的vector
            if (start >= end || start >= vecSize) {
                return std::vector<T>();
            }

            // 使用迭代器构造新vector
            auto first = vec.begin() + start;
            auto last = vec.begin() + end;
            return std::vector<T>(first, last);
        }

        template<typename T>
        static T min(T a, T b)
        {
            return a < b ? a : b;
        }

        template<typename T>
        static T max(T a, T b)
        {
            return a > b ? a : b;
        }

        /******************************************************************************************************************
         * Desc:
         * Method:    easeOutQuart
         * Returns:   float
         * Parameter: float r
         ****************************************************************************************************************/
        static float easeOutQuart(float r);

        /******************************************************************************************************************
         * Desc: https://blog.csdn.net/weixin_45726044/article/details/120151153
         *       在Javascript中：splice() 方法通过删除或替换现有元素或者原地添加新的元素来修改数组，并以数组形式返回被修改的内容。此方法会改
                 变原数组。
                 参数：
                 index —— 必需。整数，规定添加/删除项目的位置，使用负数可从数组结尾处规定位置。
                 howmany —— 必需。要删除的项目数量。如果设置为 0，则不会删除项目。
                 item1, …, itemX —— 可选。向数组添加的新项目。
                 返回值：
                 Array —— 包含被删除项目的新数组，如果有的话。

                 splice的功能在C++中可以通过几种方式实现，主要依赖于C++的标准模板库（STL），尤其是 std::vector。std::vector 提供了灵活的数
                 组管理功能，可以用来模拟JavaScript的 splice 方法。下面是一个简单的C++实现，它模拟了JavaScript中的 splice 方法的基本行为。
                 这个实现重点在于删除和插入元素，但不完全模拟所有的 splice 功能（比如返回被删除的元素），因为C++的类型系统和模板机制要求更显式
                 的处理这些情况。
         * Method:    splice
         * Returns:   void
         * Parameter: std::vector<T> & vec
         * Parameter: size_t start
         * Parameter: size_t deleteCount
         * Parameter: const std::vector<T> & itemsToAdd
         ****************************************************************************************************************/
        template<typename T>
        static void splice(std::vector<T>& vec, size_t start, size_t deleteCount, const std::vector<T>& itemsToAdd = {})
        {
            // 删除元素
            if (deleteCount > 0 && start < vec.size())
            {
                vec.erase(vec.begin() + start, vec.begin() + start + deleteCount);
            }

            // 添加元素
            if (!itemsToAdd.empty())
            {
                vec.insert(vec.begin() + start, itemsToAdd.begin(), itemsToAdd.end());
            }
        }

        /******************************************************************************************************************
         * Desc:
         * Method:    SetVectorLength
         * Returns:   void
         * Parameter: std::vector<T> & vec
         * Parameter: size_t newLength
         * Parameter: T defaultValue
         ****************************************************************************************************************/
        template<typename T>
        static void SetVectorLength(std::vector<T>& vec, size_t newLength, T defaultValue)
        {
            if (0 == newLength)
            {
                vec.clear();
                return;
            }

            auto sz = vec.size();

            if (sz > newLength)
            {
                while (vec.size() > newLength)
                    vec.pop_back();

                return;
            }
            else if (sz == newLength)
            {
                return;
            }
            else
            {
                while (vec.size() < newLength)
                    vec.emplace_back(defaultValue)
            }
        }

    private:
        /******************************************************************************************************************
         * Desc:
         * Method:    PushToVector
         * Returns:   void
         * Parameter: std::vector<T> & self
         * Parameter: T value
         ****************************************************************************************************************/
        template <typename T>
        static void PushToVector(std::vector<T>& self, T value)
        {
            self.emplace_back(value);
        }

    public:
        /******************************************************************************************************************
         * Desc:
         * Method:    PushToVector
         * Returns:   void
         * Parameter: std::vector<T> & self
         * Parameter: Args & & ... args
         ****************************************************************************************************************/
        template <typename T, typename... Args>
        static void PushToVector(std::vector<T>& self, Args&&... args)
        {
            return PushToVector(self, args...);
        }

        template<typename T>
        static void ClearInstrutivePtrVector(std::vector<boost::intrusive_ptr<T>>& ptr_vector)
        {
            std::size_t sz = ptr_vector.size();
            for (std::size_t i = 0; i < sz; ++i)
                ptr_vector[i] = nullptr;

            ptr_vector.clear();
        }

    };
}

#endif // utils_h__