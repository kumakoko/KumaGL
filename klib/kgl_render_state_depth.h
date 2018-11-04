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
/*!
* \file kgl_render_state_depth.h
* \date 2017/12/31 13:46
*
* \author www.xionggf.com
* Contact: sun_of_lover@sina.com
*
* \brief  封装了深度测试相关的渲染状态的object
*
* TODO: long description
*
* \note
*/
#ifndef kgl_render_state_depth_h__
#define kgl_render_state_depth_h__

namespace kgl
{
    class RenderStateDepth
    {
    public:
        RenderStateDepth();

        RenderStateDepth(GLboolean enable, GLenum test_func);

        /// <summary>
        /// 启用或者关闭深度测试
        /// </summary>
        /// <param name="enable">GLtrue为启用，GLfalse为关闭</param>
        inline void SetEnable(GLboolean enable)
        {
            enable_ = enable;
        }
        
        /// <summary>
        /// 获取启用或者关闭深度测试
        /// </summary>
        /// <returns>GLtrue为启用，GLfalse为关闭</returns>
        inline GLboolean GetEnable() const
        {
            return enable_;
        }

        /// <summary>
        /// 设置深度测试函数
        /// </summary>
        /// <param name="func">启用深度状态的话，设置深度测试函数</param>
        inline void SetDepthTestFunc(GLenum func)
        {
            depth_test_func_ = func;
        }

        /// <summary>
        /// 获取深度测试函数
        /// </summary>
        /// <returns>GLenum.</returns>
        inline GLenum GetDepthTestFunc() const
        {
            return depth_test_func_;
        }

        /// <summary>
        /// 使用本render state
        /// </summary>
        void Use();

        /// <summary>
        /// 查询当前时刻深度测试是否已经启用，以及对应的深度测试函数是什么，存储到一个RenderStateDepth对象中并返回
        /// </summary>
        /// <param name="rs_depth">存储当前时刻深度测试是否已经启用，以及对应的深度测试函数是什么的一个RenderStateDepth对象</param>
        static void TakeSnapshotState(RenderStateDepth& rs_depth);

        /// <summary>
        /// 根据传递进来的深度测试函数值，返回该深度函数的名字
        /// </summary>
        /// <param name="func">度测试函数值</param>
        /// <returns>该深度函数的名字</returns>
        static const char* GetTestFuncName(GLuint func);

    private:
        /// <summary>
        /// 待设置的深度测试函数值,默认为GL_LESS，如果待绘制像素z值小于当前像素z值,则绘制待绘制像素
        /// OpenGL中，最远处z为1，最近处为-1
        /// GL_NEVER：永不绘制
        /// GL_LESS：如果待绘制像素z值 < 当前像素z值，则绘制待绘制像素
        /// GL_EQUAL：如果待绘制像素z值＝当前像素z值，则绘制待绘制像素
        /// GL_LEQUAL：如果待绘制像素z值<＝当前像素z值，则绘制待绘制像素
        /// GL_GREATER ：如果待绘制像素z值>当前像素z值，则绘制待绘制像素
        /// GL_NOTEQUAL：如果待绘制像素z值<>当前像素z值，则绘制待绘制像素
        /// GL_GEQUAL：如果待绘制像素z值 >= 当前像素z值，则绘制待绘制像素
        /// GL_ALWAYS：总是绘制
        /// </summary>
        GLenum depth_test_func_ = GL_LESS;

        /// <summary>
        /// 是否启用深度测试函数
        /// </summary>
        GLboolean enable_ = GL_TRUE;
    };
}
#endif // kgl_render_state_depth_h__