﻿/**************************************************************************************************************************
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
* \file kgl_camera.h
* \date 2017/12/31 13:35
*
* \author Administrator
* Contact: user@company.com
*
* \brief
*
* TODO: 摄像机类
*
* \note
*/
#ifndef kgl_camera_h__
#define kgl_camera_h__

namespace kgl
{
    enum CameraType
    {
        ORTHO, PERSPECTIVE
    };

    enum CameraDirection
    {
        UP, DOWN, LEFT, RIGHT, FORWARD, BACK
    };

    class Camera
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="Camera"/> class.
        /// </summary>
        Camera();

        /// <summary>
        /// Finalizes an instance of the <see cref="Camera"/> class.
        /// </summary>
        ~Camera();

        /// <summary>
        /// Resets this instance.
        /// </summary>
        void Reset();

        /// <summary>
        /// Updates this instance.
        /// </summary>
        void Update();

        /// <summary>
        /// Moves the specified dir.
        /// </summary>
        /// <param name="dir">The dir.</param>
        void Move(CameraDirection dir);

        /// <summary>
        /// Changes the pitch.
        /// </summary>
        /// <param name="degrees">The degrees.</param>
        void ChangePitch(float degrees);

        /// <summary>
        /// Changes the heading.
        /// </summary>
        /// <param name="degrees">The degrees.</param>
        void ChangeYaw(float degrees);

        /// <summary>
        /// Sets the mode.
        /// </summary>
        /// <param name="cam_mode">The cam_mode.</param>
        void SetMode(CameraType cam_mode);

        /// <summary>
        /// Sets the position.
        /// </summary>
        /// <param name="pos">The position.</param>
        void SetPosition(const glm::vec3& pos);

        /// <summary>
        /// Sets the fov.
        /// </summary>
        /// <param name="fov">The fov.</param>
        void SetFOV(float fov);

        /// <summary>
        /// Sets the viewport.
        /// </summary>
        /// <param name="loc_x">The loc_x.</param>
        /// <param name="loc_y">The loc_y.</param>
        /// <param name="width">The width.</param>
        /// <param name="height">The height.</param>
        void SetViewport(int loc_x, int loc_y, int width, int height);

        /// <summary>
        /// Sets the clipping.
        /// </summary>
        /// <param name="near_clip_distance">The near_clip_distance.</param>
        /// <param name="far_clip_distance">The far_clip_distance.</param>
        void SetClipping(float near_clip_distance, float far_clip_distance);

        /// <summary>
        /// Sets the yaw angle.
        /// </summary>
        /// <param name="yaw_angle">The yaw_angle.</param>
        void SetYawAngle(float yaw_angle);

        /// <summary>
        /// Gets the yaw angle.
        /// </summary>
        /// <returns>float.</returns>
        inline float GetYawAngle() const
        {
            return yaw_angle_;
        }

        /// <summary>
        /// Sets the pitch angle.
        /// </summary>
        /// <param name="pitch_angle">The pitch_angle.</param>
        void SetPitchAngle(float pitch_angle);
        
        /// <summary>
        /// 把当前的pitch角取相反数
        /// </summary>
        inline void InvertPitchAngle()
        {
            pitch_angle_ = pitch_angle_ * -1.0f;
            is_view_matrix_changed_ = true;
            is_eular_angle_changed_ = true;
        }

        /// <summary>
        /// Gets the pitch angle.
        /// </summary>
        /// <returns>float.</returns>
        inline float GetPitchAngle() const
        {
            return pitch_angle_;
        }

        /// <summary>
        /// Initializes the view projection.
        /// </summary>
        /// <param name="camera_mod">The camera_mod.</param>
        /// <param name="camera_pos">The camera_pos.</param>
        /// <param name="pitch_angle">The pitch_angle.</param>
        /// <param name="yaw_angle">The yaw_angle.</param>
        /// <param name="fov">The fov.</param>
        /// <param name="near_clip_distance">The near_clip_distance.</param>
        /// <param name="far_clip_distance">The far_clip_distance.</param>
        void InitViewProjection(CameraType camera_mode, const glm::vec3& camera_pos, float pitch_angle = 0.0f, float yaw_angle = 180.0f, float fov = 120.f, float near_clip_distance = 0.1f, float far_clip_distance = 200.0f);

        /// <summary>
        /// Gets the mode.
        /// </summary>
        /// <returns>kgl.CameraType.</returns>
        inline CameraType GetMode() const
        {
            return camera_mode_;
        }

        /// <summary>
        /// Gets the viewport.
        /// </summary>
        /// <param name="loc_x">The loc_x.</param>
        /// <param name="loc_y">The loc_y.</param>
        /// <param name="width">The width.</param>
        /// <param name="height">The height.</param>
        void GetViewport(int* loc_x, int* loc_y, int* width, int* height);

        /// <summary>
        /// Gets the projection matrix.
        /// </summary>
        /// <returns>const glm.mat4 &.</returns>
        inline const glm::mat4& GetProjectionMatrix() const
        {
            return projection_matrix_;
        }

        /// <summary>
        /// Gets the view matrix.
        /// </summary>
        /// <returns>glm.mat4.</returns>
        inline const glm::mat4& GetViewMatrix() const
        {
            return view_matrix_;
        }

        /// <summary>
        /// Gets the position.
        /// </summary>
        /// <returns>const glm.vec3 &.</returns>
        inline const glm::vec3& GetPosition() const
        {
            return position_;
        }

        /// <summary>
        /// Sets the camera speed.
        /// </summary>
        /// <param name="speed">The speed.</param>
        inline void SetCameraSpeed(float speed)
        {
            camera_speed_ = speed;
        }

        //************************************
        // Method:    SetMaxYawDegreePerFrame
        // FullName:  kgl::Camera::SetMaxYawDegreePerFrame
        // Access:    public 
        // Returns:   void
        // Qualifier: 设置每帧最大可以调整多大的yaw角
        // Parameter: float max_degree 最大的yaw角
        //************************************
        inline void SetMaxYawDegreePerFrame(float max_degree)
        {
            max_yaw_degree_per_frame_ = max_degree;
        }

        //************************************
        // Method:    SetMaxPitchDegreePerFrame
        // FullName:  kgl::Camera::SetMaxPitchDegreePerFrame
        // Access:    public 
        // Returns:   void
        // Qualifier: 设置每帧最大可以调整多大的pitch角
        // Parameter: float max_degree 最大的pitch角
        //************************************
        inline void SetMaxPitchDegreePerFrame(float max_degree)
        {
            max_pitch_degree_per_frame_ = max_degree;
        }

		//************************************
		// Method:    GetForwardDirection
		// FullName:  kgl::Camera::GetForwardDirection
		// Access:    public 
		// Returns:   const glm::vec3&
		// Qualifier: const 获取到本摄像机的朝前向量
		//************************************
		inline const glm::vec3& GetForwardDirection() const
		{
			return forward_;
		}

		inline const glm::vec3& GetUpDirection() const
		{
			return up_;
		}

    private:
        //************************************
        // Method:    UpdateCameraVectorsByEularAngle
        // FullName:  kgl::Camera::UpdateCameraVectorsByEularAngle
        // Access:    private 
        // Returns:   void
        // Qualifier: 根据给定的欧拉角计算摄像机的forward，up，right向量，此三个向量就是观察坐标系的三个轴
        //************************************
        void UpdateCameraVectorsByEularAngle();

    private:
        /// <summary>
        /// The camera_mode_
        /// </summary>
        CameraType camera_mode_;

        /// <summary>
        /// The viewport_x_
        /// </summary>
        int viewport_x_;

        /// <summary>
        /// The viewport_y_
        /// </summary>
        int viewport_y_;

        /// <summary>
        /// The window_width_
        /// </summary>
        int window_width_;

        /// <summary>
        /// The window_height
        /// </summary>
        int window_height;

        /// <summary>
        /// The aspect_
        /// </summary>
        float aspect_;

        /// <summary>
        /// The field_of_view_
        /// </summary>
        float field_of_view_;

        /// <summary>
        /// 近截平面的z值
        /// </summary>
        float near_clip_;

        /// <summary>
        /// 远截平面的z值
        /// </summary>
        float far_clip_;

        /// <summary>
        /// 摄像机移动的速度
        /// </summary>
        float camera_speed_;

        /// <summary>
        /// The projection_matrix_
        /// </summary>
        glm::mat4 projection_matrix_;
        
        /// <summary>
        /// The view_matrix_
        /// </summary>
        glm::mat4 view_matrix_;

        /// <summary>
        /// 导航角，摄像机绕着它自身的Y轴旋转的角度
        /// </summary>
        float yaw_angle_;

        /// <summary>
        /// 仰俯角，摄像机绕着它自身的X轴旋转的角度
        /// </summary>
        float pitch_angle_;

        /// <summary>
        /// 摄像机自身的向前单位向量在世界坐标中的值
        /// </summary>
        glm::vec3 forward_;

        /// <summary>
        /// 摄像机自身的向右单位向量在世界坐标中的值
        /// </summary>
        glm::vec3 right_;
        
        /// <summary>
        /// 摄像机自身的向上单位向量在世界坐标中的值，注意和world_up_区别开来
        /// </summary>
        glm::vec3 up_;
        
        /// <summary>
        /// 摄像机所处的世界坐标系中的向上单位向量，注意和up_区别开来
        /// 这个单位向量值一般都为(0,1,0)。
        /// </summary>
        glm::vec3 world_up_;

        /// <summary>
        /// 本摄像机在世界坐标系下的坐标值
        /// </summary>
        glm::vec3 position_;

        /// <summary>
        /// 欧拉角发生变化了吗
        /// </summary>
        bool is_eular_angle_changed_ = true;

        /// <summary>
        /// The is_view_matrix_dirty_
        /// </summary>
        bool is_view_matrix_changed_ = true;

        /// <summary>
        /// 投影矩阵发生了变化了吗
        /// </summary>
        bool is_projection_matrix_changed_ = true;

        /// <summary>
        /// 视口发生变化了吗
        /// </summary>
        bool is_view_port_changed_ = true;

        /// <summary>
        /// 每一幀中最大的可以PITCH操作的度數
        /// </summary>
        float max_pitch_degree_per_frame_ = 0.1f;

        /// <summary>
        /// 每一幀中最大的可以YAW操作的度數
        /// </summary>
        float max_yaw_degree_per_frame_ = 0.1f;
    };

    typedef std::shared_ptr<Camera> CameraSPtr;
}


#endif // kgl_camera_h__