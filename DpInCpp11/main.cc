// 测试代码



#include "kgl_lib_pch.h"
#include "kgl_singleton.h"
#include "kgl_events.h"
#include <iostream>
#include <functional>

using namespace std;


struct C
{
    C(int x, float y){
    }

    void Test(){ cout << "test C" << endl; }
};

struct A
{
    int a, b;
    void print(int a, int b)
    {
        cout << "A class :"<<a << "," << b << endl;
    }
};

void print(int a, int b)
{
    cout << "global function :" << a << "," << b << endl;
}

void main()
{
    kgl::dp::Singleton<C>::MakeInstance(1, 2.34f);
    C* sc = kgl::dp::Singleton<C>::GetInstance();
    sc->Test();
    kgl::dp::Singleton<C>::DeleteInstance();
     
    kgl::dp::Events<std::function<void(int, int)>> e;
    
    A t;
    e.Connect(print);
    e.Connect([&t](int a, int b)
              {
                t.a = a; t.b = b;
                cout << "lambda function :" << a << "," << b << endl;
              });

    e.Connect(std::bind(&A::print, &t, std::placeholders::_1, std::placeholders::_2));
    int a = 123;
    int b = 456;
    e.Notify(a, b);
}