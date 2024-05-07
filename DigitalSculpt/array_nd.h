#ifndef digital_sculpt_array_nd_h__
#define digital_sculpt_array_nd_h__

#include "../klib/kgl_lib_pch.h"

namespace DigitalSculpt
{
    template<typename T>
    class IArrayND
    {
    public:
        virtual int Dimension() const = 0;
        
        std::size_t ArrayItemSize() const
        {
            return sizeof(T);
        }
    };

    typedef IArrayND<std::uint32_t> IUint32ArrayND;
}

#endif // digital_sculpt_array_nd_h__
