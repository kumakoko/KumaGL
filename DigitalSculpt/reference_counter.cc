#include "reference_counter.h"

#include <cassert>

void intrusive_ptr_add_ref(ReferenceCounter* p)
{
    assert(p);
    assert(p->ref_count_ >= 0);
    ++p->ref_count_;
}

void intrusive_ptr_release(ReferenceCounter* p)
{
    assert(p);
    assert(p->ref_count_ > 0);

    if (--p->ref_count_ == 0)
    {
        delete p;
    }
}

ReferenceCounter::ReferenceCounter() :ref_count_(0)
{
}

ReferenceCounter::ReferenceCounter(const ReferenceCounter& other)
{
}

ReferenceCounter& ReferenceCounter::operator=(const ReferenceCounter& other)
{
    return *this;
}

ReferenceCounter::~ReferenceCounter()
{
}
