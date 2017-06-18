// 摄像机类

#include "kgl_lib_pch.h"
#include "kgl_camera.h"


namespace kgl
{
    Camera::Camera() 
    {
        camera_mode_ = FREE;
        camera_up_ = glm::vec3(0, 1, 0);
        field_of_view = 45;
        rotation_quaternion = glm::quat(1, 0, 0, 0);
        camera_position_delta_ = glm::vec3(0, 0, 0);
        camera_speed_ = 0.01f;
        max_pitch_rate = 5;
        max_heading_rate = 5;
        move_camera = false;
        is_dirty_ = true;

        camera_pitch_ = 0.0f;
        camera_yaw_ = 0.0f;
    }

    Camera::~Camera()
    {
    }

    void Camera::Reset()
    {
        camera_up_ = glm::vec3(0, 1, 0);
        is_dirty_ = true;
    }

    void Camera::Update()
    {
        if (!is_dirty_)
            return;

        camera_direction_ = glm::normalize(camera_look_at_ - camera_position_);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glViewport(viewport_x, viewport_y, window_width, window_height);

        if (camera_mode_ == ORTHO)
        {
            projection_matrix_ = glm::ortho(-1.5f * float(aspect_), 1.5f * float(aspect_), -1.5f, 1.5f, -10.0f, 10.f);
            is_dirty_ = false;
        }
        else if (camera_mode_ == FREE)
        {
            // 计算透视投影矩阵
            projection_matrix_ = glm::perspective(field_of_view, aspect_, near_clip_, far_clip_);
            
            // 根据camera的向上向量(up vector)和向前向
            // 量(forward vector),计算出摄像机的向右向量（right vector）
            glm::vec3 right_axis = glm::cross(camera_direction_, camera_up_);
            // 根据当前的绕向右向量的旋转角，以及向右向量，
            // 得出这个绕向右向量旋转的旋转度四元数。
            glm::quat pitch_quat = glm::angleAxis(camera_pitch_, right_axis);
            
            // 根据当前的绕向上向量的旋转角，以及向上向量，得出这个
            // 绕向上向量旋转的旋转度四元数。
            glm::quat heading_quat = glm::angleAxis(camera_yaw_, camera_up_);

            // 联合两个旋转
            glm::quat temp = glm::cross(pitch_quat, heading_quat);
            temp = glm::normalize(temp);


            // 让当前的朝向绕着temp旋转度旋转，得出新的朝向向量
            camera_direction_ = glm::rotate(temp, camera_direction_);
            
            // 摄像机本身的位移发生了变化
            camera_position_ += camera_position_delta_;
            
            // 沿着新的朝向方向和新的位置，计算出新的look at position
            camera_look_at_ = camera_position_ + camera_direction_ * 1.0f;
            
            // 做个阻尼操作，让它平滑些
            camera_yaw_ *= .5;
            camera_pitch_ *= .5;
            camera_position_delta_ = camera_position_delta_ * .8f;

            is_dirty_ = false;
        }

        view_matrix_ = glm::lookAt(camera_position_, camera_look_at_, camera_up_);
        model = glm::mat4(1.0f);
        MVP = projection_matrix_ * view_matrix_ * model;
        glLoadMatrixf(glm::value_ptr(MVP));
    }

    void Camera::SetMode(CameraType cam_mode) 
    {
        camera_mode_ = cam_mode;
        camera_up_ = glm::vec3(0, 1, 0);
        rotation_quaternion = glm::quat(1, 0, 0, 0);
        is_dirty_ = true;
    }

    void Camera::SetPosition(glm::vec3 pos)
    {
        camera_position_ = pos;
        is_dirty_ = true;
    }

    void Camera::SetLookAt(glm::vec3 pos)
    {
        camera_look_at_ = pos;
        is_dirty_ = true;
    }

    void Camera::SetFOV(float fov) 
    {
        field_of_view = fov;
        is_dirty_ = true;
    }

    void Camera::SetViewport(int loc_x, int loc_y, int width, int height) 
    {
        viewport_x = loc_x;
        viewport_y = loc_y;
        window_width = width;
        window_height = height;
        aspect_ = static_cast<float>(width) / static_cast<float>(height);
        is_dirty_ = true;
    }

    void Camera::SetClipping(float near_clip_distance, float far_clip_distance)
    {
        near_clip_ = near_clip_distance;
        far_clip_ = far_clip_distance;
        is_dirty_ = true;
    }

    void Camera::Move(CameraDirection dir)
    {
        if (camera_mode_ == FREE) 
        {
            switch (dir) {
            case UP:
                camera_position_delta_ += camera_up_ * camera_speed_;
                break;
            case DOWN:
                camera_position_delta_ -= camera_up_ * camera_speed_;
                break;
            case LEFT:
                camera_position_delta_ -= glm::cross(camera_direction_, camera_up_) * camera_speed_;
                break;
            case RIGHT:
                camera_position_delta_ += glm::cross(camera_direction_, camera_up_) * camera_speed_;
                break;
            case FORWARD:
                camera_position_delta_ += camera_direction_ * camera_speed_;
                break;
            case BACK:
                camera_position_delta_ -= camera_direction_ * camera_speed_;
                break;
            }

            is_dirty_ = true;
        }
    }

    void Camera::ChangePitch(float degrees) 
    {
        if (degrees < -max_pitch_rate)
        {
            degrees = -max_pitch_rate;
        }
        else if (degrees > max_pitch_rate)
        {
            degrees = max_pitch_rate;
        }

        camera_pitch_ += degrees;

        if (camera_pitch_ > 360.0f)
        {
            camera_pitch_ -= 360.0f;
        }
        else if (camera_pitch_ < -360.0f)
        {
            camera_pitch_ += 360.0f;
        }

        is_dirty_ = true;
    }

    void Camera::ChangeHeading(float degrees)
    {
        if (degrees < -max_heading_rate)
        {
            degrees = -max_heading_rate;
        }
        else if (degrees > max_heading_rate) 
        {
            degrees = max_heading_rate;
        }

        if (camera_pitch_ > 90.0f && camera_pitch_ < 270.0f || (camera_pitch_ < -90.0f && camera_pitch_ > -270.0f))
        {
            camera_yaw_ -= degrees;
        }
        else 
        {
            camera_yaw_ += degrees;
        }

        if (camera_yaw_ > 360.0f)
        {
            camera_yaw_ -= 360.0f;
        }

        else if (camera_yaw_ < -360.0f)
        {
            camera_yaw_ += 360.0f;
        }

        is_dirty_ = true;
    }

    void Camera::GetViewport(int* loc_x, int* loc_y, int* width, int* height)
    {
        if (nullptr != loc_x)
            *loc_x = viewport_x;

        if (nullptr != loc_y)
            *loc_y = viewport_y;

        if (nullptr != width)
            *width = window_width;

        if (nullptr != height)
            *height = window_height;
    }

	glm::mat4 Camera::GetModelViewMatrix()
	{
		return glm::translate(view_matrix_, camera_position_);
	}
}