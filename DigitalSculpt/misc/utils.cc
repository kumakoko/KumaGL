#include "misc/utils.h"

namespace DigitalSculpt
{
    int Utils::TAG_FLAG = 1;
    int Utils::SCULPT_FLAG = 1;

    float Utils::easeOutQuart(float r) 
    {
        r = std::min(1.0f, r) - 1.0f;
        return -(r * r * r * r - 1.0f);
    }

    void Utils::tidy(std::vector<unsigned int>& array)
    {
        std::sort(array.begin(), array.end());
        auto it = std::unique(array.begin(), array.end());
        array.resize(std::distance(array.begin(), it));
    }

    unsigned int Utils::getUint32(const std::vector<unsigned char>& data, size_t offset)
    {
        auto b = getBytes(data, offset);
        return (b[0] << 0) | (b[1] << 8) | (b[2] << 16) | (b[3] << 24);
    }

    float Utils::getFloat32(const std::vector<unsigned char>& data, size_t offset)
    {
        auto b = getBytes(data, offset);
        int sign = 1 - (2 * (b[3] >> 7));
        int exponent = (((b[3] << 1) & 0xff) | (b[2] >> 7)) - 127;
        int mantissa = ((b[2] & 0x7f) << 16) | (b[1] << 8) | b[0];

        if (exponent == 128) 
        {
            if (mantissa != 0)
                return NAN;
            else
                return sign * INFINITY;
        }

        if (exponent == -127)
            return sign * mantissa * std::pow(2, -126 - 23);

        return sign * (1 + mantissa * std::pow(2, -23)) * std::pow(2, exponent);
    }

    std::string Utils::ab2str(const std::vector<unsigned char>& buf)
    {
        std::string str;

        for (size_t off = 0, abLen = buf.size(); off < abLen; off += 65535) 
        {
            size_t chunkSize = std::min<size_t>(65535, abLen - off);
            std::string subab(reinterpret_cast<const char*>(&buf[off]), chunkSize);
            str += subab;
        }
        return str;
    }

    std::vector<unsigned char> Utils::getMemory(size_t nbBytes)
    {
        static std::vector<unsigned char> pool(100000);
        if (pool.size() >= nbBytes)
            return pool;
        pool.resize(nbBytes);
        return pool;
    }
}