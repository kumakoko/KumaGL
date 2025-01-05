#include "../klib/kgl_lib_pch.h"
#include "utils.h"

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <chrono>
#include <array>
#include <unordered_map>
#include <string>
#include <cstring>
#include <memory>

namespace Utils 
{
    float linearToSRGB1(float x) 
    {
        return x < 0.0031308f ? x * 12.92f : 1.055f * std::pow(x, 1.0f / 2.4f) - 0.055f;
    }

    float sRGBToLinear1(float x) 
    {
        return x < 0.04045f ? x * (1.0f / 12.92f) : std::pow((x + 0.055f) * (1.0f / 1.055f), 2.4f);
    }

    void extend(std::unordered_map<std::string, std::string>& dest, const std::unordered_map<std::string, std::string>& src) 
    {
        for (const auto& [key, value] : src)
        {
            if (dest.find(key) == dest.end()) {
                dest[key] = value;
            }
        }
    }

    std::unordered_map<std::string, std::string> invert(const std::unordered_map<std::string, std::string>& obj) 
    {
        std::unordered_map<std::string, std::string> inv;

        for (const auto& [key, value] : obj) 
        {
            inv[value] = key;
        }

        return inv;
    }

    void replaceElement(std::vector<int>& array, int oldValue, int newValue)
    {
        auto it = std::find(array.begin(), array.end(), oldValue);
        if (it != array.end()) {
            *it = newValue;
        }
    }

    void removeElement(std::vector<int>& array, int remValue)
    {
        auto it = std::find(array.begin(), array.end(), remValue);

        if (it != array.end())
        {
            *it = array.back();
            array.pop_back();
        }
    }

    void appendArray(std::vector<int>& array1, const std::vector<int>& array2)
    {
        array1.insert(array1.end(), array2.begin(), array2.end());
    }

    bool isPowerOfTwo(int x) 
    {
        return x > 0 && (x & (x - 1)) == 0;
    }

    int nextHighestPowerOfTwo(int x)
    {
        --x;

        for (int i = 1; i < 32; i <<= 1)
        {
            x |= x >> i;
        }
        return x + 1;
    }

    void tidy(std::vector<int>& array)
    {
        std::sort(array.begin(), array.end());
        auto it = std::unique(array.begin(), array.end());
        array.erase(it, array.end());
    }

    std::vector<int> intersectionArrays(const std::vector<int>& a, const std::vector<int>& b) 
    {
        std::vector<int> result;
        std::set_intersection(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(result));
        return result;
    }

    bool littleEndian() 
    {
        uint16_t number = 0x1;
        return *reinterpret_cast<uint8_t*>(&number) == 0x1;
    }

    std::vector<uint8_t> getBytes(const std::string& data, size_t offset) 
    {
        std::vector<uint8_t> bytes(4);
        std::memcpy(bytes.data(), data.data() + offset, 4);
        return bytes;
    }

    uint32_t getUint32(const std::string& data, size_t offset)
    {
        auto bytes = getBytes(data, offset);
        return (bytes[0]) | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24);
    }

    float getFloat32(const std::string& data, size_t offset)
    {
        auto bytes = getBytes(data, offset);
        int32_t intValue = getUint32(data, offset);
        float value;
        std::memcpy(&value, &intValue, sizeof(float));
        return value;
    }

    std::string ab2str(const std::vector<uint8_t>& buffer) 
    {
        return std::string(buffer.begin(), buffer.end());
    }

    std::unique_ptr<std::vector<uint8_t>> getMemory(size_t nbBytes)
    {
        static std::vector<uint8_t> pool(100000);

        if (pool.size() >= nbBytes) 
        {
            return std::make_unique<std::vector<uint8_t>>(pool.begin(), pool.begin() + nbBytes);
        }

        return std::make_unique<std::vector<uint8_t>>(nbBytes);
    }

    int64_t now()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    }

    // Throttling functionality can be implemented using function wrappers if needed

    void normalizeArrayVec3(vec3& array, vec3& arrayOut)
    {
        for (size_t i = 0; i < array.size(); i += 3) 
        {
            float nx = array[i], ny = array[i + 1], nz = array[i + 2];
            float len = std::sqrt(nx * nx + ny * ny + nz * nz);
            if (len == 0.0f) {
                arrayOut[i] = 1.0f;
                arrayOut[i + 1] = 0.0f;
                arrayOut[i + 2] = 0.0f;
                continue;
            }
            len = 1.0f / len;
            arrayOut[i] = nx * len;
            arrayOut[i + 1] = ny * len;
            arrayOut[i + 2] = nz * len;
        }
    }

    void convertArrayVec3toSRGB(vec3& array, vec3& arrayOut)
    {
        for (size_t i = 0; i < array.size(); ++i)
        {
            arrayOut[i] = linearToSRGB1(array[i]);
        }
    }

    void convertArrayVec3toLinear(vec3& array, vec3& arrayOut) 
    {
        for (size_t i = 0; i < array.size(); ++i)
        {
            arrayOut[i] = sRGBToLinear1(array[i]);
        }
    }
} // namespace Utils
