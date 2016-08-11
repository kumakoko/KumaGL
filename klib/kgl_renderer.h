// 渲染器类
#ifndef kgl_renderer_h__
#define kgl_renderer_h__

namespace kgl
{
	class Renderer
	{
	public:
		Renderer();
		~Renderer();
		virtual void Init(bool use_experimental);
		virtual void Clear();
	};

	typedef std::shared_ptr<Renderer> RendererPtr;
}

#endif // kgl_renderer_h__
