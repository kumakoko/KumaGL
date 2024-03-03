#ifndef digital_sculpt_array_nd_h__
#define digital_sculpt_array_nd_h__

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
}

#endif // digital_sculpt_array_nd_h__
