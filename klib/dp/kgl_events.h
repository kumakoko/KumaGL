// 用于观察者模式的事件类
#ifndef kgl_events_h__
#define kgl_events_h__

namespace kgl
{
    namespace dp
    {
        // 在实际使用中，模板参数将会是一个std::function的实例化对象，也即是观察者将要被调用通知的那个接口函数
        template<typename Func>
        class Events : boost::noncopyable
        {
        public:
            Events(){}
            ~Events(){};

            // 注册观察者，支持右值调用
            int Connect(Func&& f)
            {
                return this->Assign(f);
            }

            int Connect(const Func& f)
            {
                return this->Assign(f);
            }

            // 移除观察者
            void Disconnect(int key)
            {
                connections_.erase(key);
            }

            template<typename... Args>
            void Notify(Args&&...args)
            {
                for (auto it : connections_)
                    it.second(std::forward<Args>(args)...);
            }
        private:
            // 保存观察者并分配观察者的编号
            template<typename F>
            int Assign(F&& f)
            {
                int k = observer_id_++;
                connections_.emplace(k, std::forward<F>(f));
                return k;
            }

            int observer_id_ = 0; // 使用直接初始化的C++语法
            std::map<int, Func> connections_;
        };
    }
}

#endif // kgl_events_h__