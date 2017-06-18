// 封装了透明混合相关的渲染状态的object
namespace kgl
{
    enum BlendFunc
    {
        BF_SRC_ALPHA = GL_SRC_ALPHA,
        BF_SRC_COLOR = GL_SRC_COLOR,
        BF_ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
        BF_ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
        BF_DST_ALPHA = GL_DST_ALPHA,
        BF_ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
        BF_DST_COLOR = GL_DST_COLOR,
        BF_ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,
        BF_SRC_ALPHA_SATURATE = GL_SRC_ALPHA_SATURATE
    };

    class RenderStateBlend
    {
    public:
        RenderStateBlend();
        void Enable(bool enable);
        void SetBlendFunction(BlendFunc src_func, BlendFunc dst_func);
    };
}