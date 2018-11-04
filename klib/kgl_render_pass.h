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
* \file kgl_render_pass.h
* \date 2017/12/31 13:57
*
* \author www.xionggf.com
* Contact: sun_of_lover@sina.com
*
* \brief 封装了render pass的基本属性和操作，可以作为具体的render pass的基类以作继承
*
* TODO: 一个render pass含有一个camera，一个gpu program
*       提供了若干常用的uniform变量传递接口：
*       world matrix, view matrix, projection matrix
* \note
*/

#ifndef kgl_render_pass_h__
#define kgl_render_pass_h__

#include "kgl_camera.h"
#include "kgl_gpu_program.h"

namespace kgl
{
    class RenderPass
    {
    public:
        /// <summary>
        /// 构造函数，创建一个 <see cref="RenderPass"/> 类的对象实例时被调用.
        /// </summary>
        RenderPass();

        /// <summary>
        /// 析构函数，删除一个<see cref="RenderPass"/>类的对象实例时被调用.
        /// </summary>
        virtual ~RenderPass();

        /// <summary>
        /// 向本render pass绑定一个主摄像机
        /// </summary>
        /// <param name="camera">The camera.</param>
        void AttachCamera(CameraSPtr camera);
        
        /// <summary>
        /// 给本render pass绑定一个主shadow
        /// </summary>
        /// <param name="program"></param>
        void AttachGPUProgram(GPUProgramSPtr program);

        /// <summary>
        /// Creates the and attach gpu program.
        /// </summary>
        /// <param name="vs_file">The vs_file.</param>
        /// <param name="fs_file">The fs_file.</param>
        /// <param name="gs_file">The gs_file.</param>
        void CreateAndAttachGPUProgram(const GLchar* vs_file, const GLchar* fs_file, const GLchar* gs_file = nullptr);

        /// <summary>
        /// Initializes this instance.
        /// </summary>
        virtual void Initialize();

        /// <summary>
        /// Sets the world matrix.
        /// </summary>
        /// <param name="world_matrix">The world_matrix.</param>
        /// <param name="need_transpose">The need_transpose.</param>
        virtual void SetWorldMatrix(const glm::mat4& world_matrix, bool need_transpose = false);

        /// <summary>
        /// Sets the view matrix.
        /// </summary>
        /// <param name="view_matrix">The view_matrix.</param>
        /// <param name="need_transpose">The need_transpose.</param>
        virtual void SetViewMatrix(const glm::mat4& view_matrix, bool need_transpose = false);

        /// <summary>
        /// Sets the projection matrix.
        /// </summary>
        /// <param name="projection_matrix">The projection_matrix.</param>
        /// <param name="need_transpose">The need_transpose.</param>
        virtual void SetProjectionMatrix(const glm::mat4& projection_matrix, bool need_transpose = false);

        /// <summary>
        /// Sets the camera world position.
        /// </summary>
        /// <param name="world_position">The world_position.</param>
        virtual void SetCameraWorldPosition(const glm::vec3& world_position);

        /// <summary>
        /// Updates the specified current_time.
        /// </summary>
        /// <param name="current_time">The current_time.</param>
        virtual void Update(float current_time);

        virtual void PrepareRendering();
    protected:
        /// <summary>
        /// Gets the world matrix location.
        /// </summary>
        /// <param name="uniform_name">The uniform_name.</param>
        void GetWorldMatrixLocation(const char* uniform_name);

        /// <summary>
        /// Gets the view matrix location.
        /// </summary>
        /// <param name="uniform_name">The uniform_name.</param>
        void GetViewMatrixLocation(const char* uniform_name);

        /// <summary>
        /// Gets the projection matrix location.
        /// </summary>
        /// <param name="uniform_name">The uniform_name.</param>
        void GetProjectionMatrixLocation(const char* uniform_name);

        /// <summary>
        /// Gets the camera world position location.
        /// </summary>
        /// <param name="uniform_name">The uniform_name.</param>
        void GetCameraWorldPositionLocation(const char* uniform_name);
    protected:
        /// <summary>
        /// 执行本render pass要使用到的camera
        /// </summary>
        CameraSPtr camera_;

        /// <summary>
        /// 执行本render pass要使用到的gpu program
        /// </summary>
        GPUProgramSPtr gpu_program_;

        /// <summary>
        /// The location_world_matrix_
        /// </summary>
        GLint location_world_matrix_ = -1;

        /// <summary>
        /// The location_project_matrix
        /// </summary>
        GLint location_projection_matrix_ = -1;

        /// <summary>
        /// The location_view_matrix
        /// </summary>
        GLint location_view_matrix_ = -1;

        GLint location_camera_world_position_ = -1;
    };

    typedef std::shared_ptr<RenderPass> RenderPassSPtr;
}

#endif // kgl_render_pass_h__