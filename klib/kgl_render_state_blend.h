// 封装了透明混合相关的渲染状态的object
#ifndef kgl_render_state_blend_h__
#define kgl_render_state_blend_h__

namespace kgl
{
    class RenderStateBlend
    {
    public:
        /// <summary>
        /// 实例化一个<see cref="RenderStateBlend"/> 类对象出来
        /// </summary>
        RenderStateBlend();

        /// <summary>
        /// 根据给定的参数实例化一个 <see cref="RenderStateBlend"/> 类对象出来
        /// </summary>
        /// <param name="enable">启用混合吗</param>
        /// <param name="src_factor">源混合因子</param>
        /// <param name="dst_factor">目标混合因子</param>
        RenderStateBlend(GLboolean enable, GLenum src_factor, GLenum dst_factor);

        /// <summary>
        /// 启用或者关闭混合 
        /// </summary>
        /// <param name="enable">The enable.</param>
        void SetEnable(GLboolean enable);

        /// <summary>
        /// 设置源混合因子和目标混合因子
        /// </summary>
        /// <param name="src_func">源混合因子</param>
        /// <param name="dst_func">目标混合因子.</param>
        void SetBlendFunction(GLenum src_factor, GLenum dst_factor);

        /// <summary>
        /// 使用本渲染状态
        /// </summary>
        void Use();

        /// <summary>
        /// Takes the state of the snapshot.
        /// </summary>
        /// <param name="blend_mode">The blend_mode.</param>
        static void TakeSnapshotState(RenderStateBlend& blend_mode);

    private:
        /// <summary>
        /// 源混合因子
        /// </summary>
        GLenum src_factor_ = GL_SRC_ALPHA;

        /// <summary>
        /// 目标混合因子
        /// </summary>
        GLenum dst_factor_ = GL_ONE_MINUS_SRC_ALPHA;

        /// <summary>
        /// 是否启用alpha blend
        /// </summary>
        GLboolean enable_ = GL_TRUE;
    };
}

#endif // kgl_render_state_blend_h__