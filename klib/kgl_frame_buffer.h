// 封装了帧缓冲区的类

namespace kgl
{
    class FrameBuffer
    {
    public:
        //************************************
        // Method:    FrameBuffer
        // FullName:  kgl::FrameBuffer::FrameBuffer
        // Access:    public 
        // Returns:   
        // Qualifier:
        // 构造函数
        //************************************
        FrameBuffer();

        //************************************
        // Method:    ~FrameBuffer
        // FullName:  kgl::FrameBuffer::~FrameBuffer
        // Access:    virtual public 
        // Returns:   
        // Qualifier:
        // 析构函数
        //************************************
        virtual ~FrameBuffer();

        //************************************
        // Method:    Create
        // FullName:  kgl::FrameBuffer::Create
        // Access:    public 
        // Returns:   void
        // Qualifier:
        // 创建frame buffer
        //************************************
        void Create();
        
        //************************************
        // Method:    GetBufferID
        // FullName:  kgl::FrameBuffer::GetBufferID
        // Access:    public 
        // Returns:   GLuint
        // Qualifier: const
        // 返回frame buffer对象ID
        //************************************
        inline GLuint GetBufferID() const
        {
            return fbo_;
        }

        //************************************
        // Method:    ReleaseBuffer
        // FullName:  kgl::FrameBuffer::ReleaseBuffer
        // Access:    public 
        // Returns:   void
        // Qualifier: 
        // 释放frame buffer对象
        //************************************
        void ReleaseBuffer();
    protected:
        GLuint fbo_;
    };
}
