#ifndef utils_h__
#define utils_h__

#include "jsm_type_defines.h"

using namespace jsm;

namespace Utils
{
    constexpr float SCALE = 100.0f;
    constexpr int TAG_FLAG = 1;
    constexpr int SCULPT_FLAG = 1;
    constexpr int STATE_FLAG = 1;
    constexpr uint32_t TRI_INDEX = 4294967295;

    float linearToSRGB1(float x);

    float sRGBToLinear1(float x);

    void extend(std::unordered_map<std::string, std::string>& dest, const std::unordered_map<std::string, std::string>& src);

    std::unordered_map<std::string, std::string> invert(const std::unordered_map<std::string, std::string>& obj);

    void replaceElement(std::vector<int>& array, int oldValue, int newValue);

    void removeElement(std::vector<int>& array, int remValue);

    void appendArray(std::vector<int>& array1, const std::vector<int>& array2);

    bool isPowerOfTwo(int x);

    int nextHighestPowerOfTwo(int x);

    void tidy(std::vector<int>& array);

    std::vector<int> intersectionArrays(const std::vector<int>& a, const std::vector<int>& b);

    bool littleEndian();

    std::vector<uint8_t> getBytes(const std::string& data, size_t offset);

    uint32_t getUint32(const std::string& data, size_t offset);

    float getFloat32(const std::string& data, size_t offset);

    std::string ab2str(const std::vector<uint8_t>& buffer);

    std::unique_ptr<std::vector<uint8_t>> getMemory(size_t nbBytes);

    int64_t now();

    // Throttling functionality can be implemented using function wrappers if needed

    void normalizeArrayVec3(vec3& array, vec3& arrayOut);

    void convertArrayVec3toSRGB(vec3& array, vec3& arrayOut);

    void convertArrayVec3toLinear(vec3& array, vec3& arrayOut);

} // namespace Utils

#endif // utils_h__