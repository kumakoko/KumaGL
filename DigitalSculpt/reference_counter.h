#ifndef reference_counter_h__
#define reference_counter_h__

#include <atomic>

namespace DigitalSculpt
{
    class ReferenceCounter
    {
    public:
        friend void intrusive_ptr_add_ref(ReferenceCounter* p);

        friend void intrusive_ptr_release(ReferenceCounter* p);

        ReferenceCounter();

        ReferenceCounter(const ReferenceCounter& other);

        ReferenceCounter& operator=(const ReferenceCounter& other);

        virtual ~ReferenceCounter();

        inline int RefCount()
        {
            return ref_count_;
        }

    private:
        std::atomic_int ref_count_;
    };
}

#endif // reference_counter_h__
