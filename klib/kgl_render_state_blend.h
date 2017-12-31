// 封装了透明混合相关的渲染状态的object
/*!
 * \file kgl_render_state_blend.h
 * \date 2017/12/31 13:46
 *
 * \author www.xionggf.com
 * Contact: sun_of_lover@sina.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
/**************************************************************************************************************************
Copyright(C) 2014-2017 www.xionggf.com

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute,sublicense, and/or sell copies of the Software, and to permit persons to whom the 
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
ARISING FROM,OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************************************/
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