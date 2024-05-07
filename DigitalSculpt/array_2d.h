#ifndef digital_sculpt_array_2d_h__
#define digital_sculpt_array_2d_h__

#include "../klib/kgl_lib_pch.h"
#include "array_nd.h"

namespace DigitalSculpt
{
    template<typename T>
    class Array2D : public IArrayND<T>
    {
    private:
        T** _Array;
    public:
        virtual int Dimension() const override
        {
            return 2;
        }

        Array2D(int elementCount)
        {
            _Array = new T * [elementCount];
            for (int i = 0; i < elementCount; ++i)
                _Array[i] = nullptr;
        }

        T*& operator [](int index)
        {
            return _Array[index];
        }

        const T*& operator [](int index) const
        {
            return _Array[index];
        }
    };

    typedef Array2D<std::uint32_t> Uint32Array2D;
}

#endif // digital_sculpt_array_2d_h__