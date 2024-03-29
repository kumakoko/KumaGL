
#ifndef test_boost_Intrusive_ptr_h__
#define test_boost_Intrusive_ptr_h__


#include <cassert>
#include <atomic>
#include <iostream>
#include <vector>

#include "boost/intrusive_ptr.hpp"

class ReferenceCounter
{
public:
    friend void intrusive_ptr_add_ref(ReferenceCounter* p)
    {
        assert(p);
        assert(p->ref_count >= 0);
        ++p->ref_count;
        std::cout << "Call intrusive_ptr_add_ref.Current REF counter = "<<p->ref_count << std::endl;
    }

    friend void intrusive_ptr_release(ReferenceCounter* p)
    {
        assert(p);
        assert(p->ref_count > 0);
        if (--p->ref_count == 0)
        {
            std::cout << "Call intrusive_ptr_release_ref.Current REF counter = " << p->ref_count << std::endl;
            delete p;
        }
        else
        {
            std::cout << "Call intrusive_ptr_release_ref.Current REF counter = " << p->ref_count << std::endl;
        }
    }

    ReferenceCounter() :ref_count(0)
    {
        std::cout << "Reference Counter Constructor" << std::endl;
    }

    ReferenceCounter(const ReferenceCounter& other)
    {
        std::cout << "Reference Counter Copy Constructor" << std::endl;
    }

    ReferenceCounter& operator=(const ReferenceCounter& other)
    {
        std::cout << "Reference Counter Assignment Operator" << std::endl;
    }

    virtual ~ReferenceCounter()
    {
        std::cout << "Reference Counter Destructor" << std::endl;
    };

    int RefCount()
    {
        return ref_count;
    }

private:
    std::atomic_int ref_count;
};

class ProcessData : public ReferenceCounter
{
public:
    ProcessData(int id, std::string info) :m_id(id), m_info(info)
    {
        std::cout << "Process Data Constructor. Current REF counter = " << RefCount() << std::endl;
    }

    ProcessData(const ProcessData& other)
    {
        std::cout << "Process Data Copy Constructor. Current REF counter = " << RefCount() << std::endl;
        m_id = other.m_id;
        m_info = other.m_info;
    }

    const ProcessData operator=(const ProcessData& other)
    {
        std::cout << "Process Data Assignment Operator" << std::endl;
        m_id = other.m_id;
        m_info = other.m_info;
    }

    virtual ~ProcessData()
    {
        std::cout << "Process Data Destructor.Current REF counter = " << RefCount() << std::endl;
    }

private:
    int m_id;
    std::string m_info;
};

typedef boost::intrusive_ptr<ProcessData> ProcessDataIPtr;

class Holder
{
    ProcessDataIPtr _ptr;
public:
    Holder()
    {
        _ptr = new ProcessData(1, "a");
    }

    ~Holder()
    {
        //_ptr = nullptr;
    }
};

void TestBoostIntrusivePtr()
{
    //ProcessDataIPtr ptr(new ProcessData(1, "a"));
    //std::cout << "******************" << std::endl;
    //std::cout << "ref_count = " << ptr->RefCount() << std::endl;

    //std::vector<ProcessDataIPtr>  pds;
    //pds.push_back(ptr);
    //pds.clear();

    Holder _ptr;

    //std::cout << "ref_count = " << ptr->RefCount() << std::endl;
}


#endif // test_boost_Intrusive_ptr_h__
