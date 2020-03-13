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
* \file kgl_gbuffer.h
* \date 2017/12/27 11:26
*
* \author Administrator
* Contact: user@company.com
*
* \brief
*
* TODO: long description
*
* \note
*/

#ifndef kgl_gbuffer_h__
#define kgl_gbuffer_h__

namespace kgl
{
    class GBuffer
    {
    public:
		/*********************************************************
		类的构造函数
		*********************************************************/
        GBuffer();

		/*********************************************************
		类的析构函数
		*********************************************************/
        ~GBuffer();
        
        /*********************************************************
        根据给定的窗口高宽，初始化G-buffer对象及其对应的缓冲区

        @param  GLuint window_width  窗口宽度
        @param  GLuint window_height 窗口高度
		@param  bool is_pos_depth_together 记录位置信息的buffer，是否把深度信息一起存在这个buffer中
        @return  创建成功返回true，失败返回false      
        *********************************************************/
        void Initialise(GLuint window_width, GLuint window_height,bool is_pos_depth_together = false);
        
        /*********************************************************
		启动GBuffer的render pass       
        *********************************************************/
        void StartGeometryRenderPass();

		/*********************************************************
		启动光照计算的render pass
		*********************************************************/
        void StartLightingRenderPass();

        void Blit();

		/*********************************************************
		结束GBuffer的render pass
		*********************************************************/
        void EndGeometryRenderPass();
    private:
		/** * @brief FRAME BUFER OBJECT的id */
        GLuint fbo_;

		/** * @brief 纪录位置信息的BUFFER的id */
        GLuint position_info_texture_ = 0;

		/** * @brief 纪录法线信息的BUFFER的id */
        GLuint normal_info_texture_ = 0;

		/** * @brief 纪录反照率信息的BUFFER的id */
        GLuint albedo_info_texture_ = 0;

		/** * @brief GBUFFER的宽度 */
        GLuint window_width_ = 0;

		/** * @brief GBUFFER的高度 */
        GLuint window_height_ = 0;      
    };
}

#endif // kgl_gbuffer_h__
