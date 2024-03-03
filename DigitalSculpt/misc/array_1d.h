#ifndef digital_sculpt_array_1d_h__
#define digital_sculpt_array_1d_h__

#include "./misc/array_nd.h"

namespace DigitalSculpt
{
    template<typename T>
    class Array1D : public IArrayND<T>
    {
    private:
        T* _Array;
    public:
        T* Array()
        {
            return _Array;
        }

        const T* Array() const
        {
            return _Array;
        }

        Array1D(int count)
        {
            _Array = new T[count];
        }

        Array1D(int count, T fillElements)
        {
            _Array = new T[count];
            memset(_Array, fillElements, sizeof(T) * count);
        }

        Array1D(const T* srcArray)
        {
            auto bufLen = sizeof(srcArray);
            auto elementCount = bufLen / sizeof(T);
            _Array = new T[elementCount];

            for (auto i = 0; i < elementCount; ++i)
                _Array[i] = srcArray[i];

            /*
            uint8_t* tmp = new uint8_t[bufLen];
            memcpy(tmp, srcArray, bufLen);
            _Array = reinterpret_cast<T*>(tmp);
            */
        }

        T& operator [](int index)
        {
            return _Array[index];
        }

        const T& operator [](int index) const
        {
            return _Array[index];
        }

        virtual int Dimension() const override
        {
            return 1;
        }
    };
}

#endif // digital_sculpt_array_1d_h__