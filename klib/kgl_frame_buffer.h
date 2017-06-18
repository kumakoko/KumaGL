// 封装了帧缓冲区的类

namespace kgl
{
    class FrameBuffer
    {
    public:
		/// <summary>
		/// 构造函数，创建一个 <see cref="FrameBuffer"/> 类的对象实例时被调用.
		/// </summary>
		FrameBuffer();

		/// <summary>
		/// 析构函数，删除一个<see cref="FrameBuffer"/>类的对象实例时被调用.
		/// </summary>
		virtual ~FrameBuffer();

		/// <summary>
		/// 创建一个frame buffer object
		/// </summary>
		void Create();
        
		/// <summary>
		/// 返回frame buffer object的ID
		/// </summary>
		/// <returns>返回frame buffer对象ID</returns>
		inline GLuint GetBufferID() const
        {
            return fbo_;
        }

		/// <summary>
		/// 执行绑定
		/// </summary>
		inline void Bind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
		}

		/// <summary>
		/// 解除绑定
		/// </summary>
		inline void Unbind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		/// <summary>
		/// 释放frame buffer对象
		/// </summary>
		void ReleaseBuffer();
    protected:
		/// <summary>
		/// frame buffer object的ID值，由OpenGL管线创建
		/// </summary>
		GLuint fbo_;
    };
}
