// 单例类，支持多参数的构造函数定义
// 本单例类并没有考虑线程安全，如有多线程场合使用单例模式的需求，请考虑
// 使用boost库的singleton
/*

http://blog.csdn.net/tiandyoin/article/details/43604055

std::forward的源代码如下：
// TEMPLATE CLASS identity
template<class _Ty>
struct identity
{    
    // map _Ty to type unchanged
    typedef _Ty type;

    const _Ty& operator()(const _Ty& _Left) const
    {
        // apply identity operator to operand
        return (_Left);
    }
};

// TEMPLATE FUNCTION forward
template<class _Ty> inline
_Ty&& forward(typename identity<_Ty>::type& _Arg)
{    
    // forward _Arg, given explicitly specified type parameter
    return ((_Ty&&)_Arg);
}

其含义是：

简单来说，identity 用于告诉编译器，我不要你帮我推导类型了(写成式子就是 forward(_val)， 不带 <_Ty> 的 )，
我自己明确指定类型 ( identity<_Ty>::type)。这样在传入参数时， 编译器就不会自己猜想：把 _arg 想成左值了
（折叠规则，这个规则是写这个编译器的人规定的，我们就不要费力去问为什么要这样做了，如果你偏要问，那
你可以自己定义规则，这样就变成了你自己的编译器了）。


引用折叠规则如下：
X& + & => X&        (1)
X&& + & => X&       (2)
X& + && => X&       (3)
X&& + && => X&&     (4)

outer(X&& t)
{
    inner(t)
}

inner(X&&)如2， 意思是把外部右值X&& t 传给函数 inner(X&&), 到了inner函数里， a就变认为是左值了，即 X&.给它变成

inner( _STD forward<_Ty>(t))

那么这时编译器执行函数inner前 ， 会对传给t的类型 进行推导， 而当指明类型为 identity<_Ty>::type, 编译器放弃
了自动推导，认为调用者已经明确指定了类型为 ...type, 当然forward返回的类型也要为 右值 X&&。你也许会问为什么要
为X&&, 而不为 X&. 这不废话吗，旧的编译器由于推导机制会把 右值认为成左值，不会把左值认为是右值，使用新的forward，
就是为了保持右值的性质不变。
*/

#ifndef kgl_singleton_h__
#define kgl_singleton_h__

namespace kgl
{
    namespace dp
    {
        template<typename T>
        class Singleton
        {
        
        public:
            template<typename... Args>
            static T* MakeInstance(Args&&... args)
            {
                if (nullptr == s_instance_)
                    s_instance_ = new T(std::forward<Args>(args)...);

                return s_instance_;
            }

            static T* GetInstance()
            {
                if (nullptr == s_instance_)
                    throw std::logic_error("The instance is not initialized,please initialize the instance by invoking static member function MakeInstance first!");

                return s_instance_;
            }

            static void DeleteInstance()
            {
                if (s_instance_ != nullptr)
                {
                    delete s_instance_;
                    s_instance_ = nullptr;
                }
            }

        private:
            Singleton();

            // 将析构函数定义为私有，表明本类只允许从堆中创建对象实例
            virtual ~Singleton();

            // 不允许拷贝构造
            Singleton(const Singleton& s);
            Singleton& operator = (const Singleton& s);

        private:
            static T* s_instance_;
        };

        template<typename T>
        T* Singleton<T>::s_instance_ = nullptr;
    }
}

#endif // kgl_singleton_h__

