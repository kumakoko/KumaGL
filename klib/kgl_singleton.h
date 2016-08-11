// 全局单例类模板
#ifndef kgl_singleton_h__
#define kgl_singleton_h__

namespace kgl
{
	template<typename T>
	class Singleton : public boost::noncopyable
	{
	public:
		static inline T* Instance();
		static std::auto_ptr<T> s_instance_;
	};

	template <class T>
	std::auto_ptr<T> Singleton<T>::s_instance_;

	template <class T>
	inline T* Singleton<T>::Instance()
	{
		if (nullptr == s_instance_.get())
		{
			s_instance_.reset(new T);
		}

		return s_instance_.get();
	}

#define DECLARE_SINGLETON_CLASS( type ) \
       friend class std::auto_ptr< type >;\
       friend class kgl::Singleton< type >;
}

#endif // kgl_singleton_h__