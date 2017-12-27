// 摄像机类

#include "kgl_lib_pch.h"
#include "kgl_camera.h"


namespace kgl
{
    Camera::Camera() 
    {
        world_up_ = glm::vec3(0, 1, 0);
        field_of_view_ = 45.0f;
        camera_speed_ = 0.1f;
        pitch_angle_ = 0.0f;
        yaw_angle_ = 0.0f;
    }

    Camera::~Camera()
    {
    }

    void Camera::Reset()
    {
        world_up_ = glm::vec3(0.f, 1.f, 0.f);
        is_view_matrix_changed_ = true;
        is_projection_matrix_changed_ = true;
        is_view_port_changed_ = true;
    }

    void Camera::Update()
    {
        if (is_view_port_changed_)
        {
            glViewport(viewport_x_, viewport_y_, window_width_, window_height);
            is_view_port_changed_ = false;
        }

        if (is_projection_matrix_changed_)
        {
            if (camera_mode_ == ORTHO)
            {
                // 计算正交投影矩阵
                projection_matrix_ = glm::ortho(-1.5f * float(aspect_), 1.5f * float(aspect_), -1.5f, 1.5f, -10.0f, 10.f);
            }
            else
            {
                // 计算透视投影矩阵
                projection_matrix_ = glm::perspective(field_of_view_, aspect_, near_clip_, far_clip_);
            }

            is_projection_matrix_changed_ = false;
        }

        if (is_view_matrix_changed_)
        {
            if (is_eular_angle_changed_)
            {
                this->UpdateCameraVectorsByEularAngle();
                is_eular_angle_changed_ = false;
            }

            // 更新观察矩阵
            view_matrix_ = glm::lookAt(position_, position_ + forward_, up_);
            is_view_matrix_changed_ = false;
        }
    }

    void Camera::SetMode(CameraType cam_mode) 
    {
        camera_mode_ = cam_mode;
        world_up_ = glm::vec3(0, 1, 0);
        is_view_matrix_changed_ = true;
        is_projection_matrix_changed_ = true;
    }

    void Camera::SetPosition(const glm::vec3& pos)
    {
        position_ = pos;
        is_view_matrix_changed_ = true;
    }

    void Camera::SetFOV(float fov) 
    {
        field_of_view_ = fov;
        is_projection_matrix_changed_ = true;
    }

    void Camera::SetViewport(int loc_x, int loc_y, int width, int height) 
    {
        viewport_x_ = loc_x;
        viewport_y_ = loc_y;
        window_width_ = width;
        window_height = height;
        aspect_ = static_cast<float>(width) / static_cast<float>(height);
        is_projection_matrix_changed_ = true;
        is_view_port_changed_ = true;
    }

    void Camera::SetClipping(float near_clip_distance, float far_clip_distance)
    {
        near_clip_ = near_clip_distance;
        far_clip_ = far_clip_distance;
        is_projection_matrix_changed_ = true;
    }

    void Camera::InitViewProjection(CameraType camera_mode, const glm::vec3& camera_pos, float pitch_angle /*= 0.0f*/, float yaw_angle /*= 0.0f*/, float fov /*= 120.f*/, float near_clip_distance /*= 0.1f*/, float far_clip_distance /*= 200.0f*/)
    {
        this->SetMode(camera_mode);
        this->SetPosition(camera_pos);
        this->SetPitchAngle(pitch_angle);
        this->SetYawAngle(yaw_angle);
        this->SetFOV(fov);
        this->SetClipping(near_clip_distance, far_clip_distance);
    }

    void Camera::SetYawAngle(float yaw_angle)
    {
        yaw_angle_ = yaw_angle;
        is_view_matrix_changed_ = true;
        is_eular_angle_changed_ = true;
    }

    void Camera::SetPitchAngle(float pitch_angle)
    {
        pitch_angle_ = pitch_angle;
        is_view_matrix_changed_ = true;
        is_eular_angle_changed_ = true;
    }

    void Camera::Move(CameraDirection dir)
    {
        if (camera_mode_ == PERSPECTIVE) 
        {
            switch (dir) 
            {
            case UP:
                position_ += up_ * camera_speed_;
                break;
            case DOWN:
                position_ -= up_ * camera_speed_;
                break;
            case LEFT:
                position_ -= right_ * camera_speed_;
                break;
            case RIGHT:
                position_ += right_ * camera_speed_;
                break;
            case FORWARD:
                position_ += forward_ * camera_speed_;
                break;
            case BACK:
                position_ -= forward_ * camera_speed_;
                break;
            }

            is_view_matrix_changed_ = true;
        }
    }

    void Camera::ChangePitch(float degrees) 
    {
        if (degrees < -max_pitch_degree_per_frame_)
        {
            degrees = -max_pitch_degree_per_frame_;
        }
        else if (degrees > max_pitch_degree_per_frame_)
        {
            degrees = max_pitch_degree_per_frame_;
        }

        pitch_angle_ += degrees;

        if (pitch_angle_ > 360.0f)
        {
            pitch_angle_ -= 360.0f;
        }
        else if (pitch_angle_ < -360.0f)
        {
            pitch_angle_ += 360.0f;
        }

        is_view_matrix_changed_ = true;
        is_eular_angle_changed_ = true;
    }

    void Camera::ChangeYaw(float degrees)
    {
        if (degrees < -max_yaw_degree_per_frame_)
        {
            degrees = -max_yaw_degree_per_frame_;
        }
        else if (degrees > max_yaw_degree_per_frame_)
        {
            degrees = max_yaw_degree_per_frame_;
        }

        if (pitch_angle_ > 90.0f && pitch_angle_ < 270.0f || (pitch_angle_ < -90.0f && pitch_angle_ > -270.0f))
        {
            yaw_angle_ -= degrees;
        }
        else 
        {
            yaw_angle_ += degrees;
        }

        if (yaw_angle_ > 360.0f)
        {
            yaw_angle_ -= 360.0f;
        }

        else if (yaw_angle_ < -360.0f)
        {
            yaw_angle_ += 360.0f;
        }

        is_view_matrix_changed_ = true;
        is_eular_angle_changed_ = true;
    }

    void Camera::GetViewport(int* loc_x, int* loc_y, int* width, int* height)
    {
        if (nullptr != loc_x)
            *loc_x = viewport_x_;

        if (nullptr != loc_y)
            *loc_y = viewport_y_;

        if (nullptr != width)
            *width = window_width_;

        if (nullptr != height)
            *height = window_height;
    }

    void Camera::UpdateCameraVectorsByEularAngle()
    {
        /*
        forward_.x = cosf(glm::radians(yaw_angle_)) * cosf(glm::radians(pitch_angle_));
        forward_.y = sinf(glm::radians(pitch_angle_));
        forward_.z = sinf(glm::radians(yaw_angle_)) * cosf(glm::radians(pitch_angle_));
        */

        forward_.z = cosf(glm::radians(yaw_angle_)) * cosf(glm::radians(pitch_angle_));
        forward_.y = sinf(glm::radians(pitch_angle_));
        forward_.x = sinf(glm::radians(yaw_angle_)) * cosf(glm::radians(pitch_angle_));

        forward_ = glm::normalize(forward_);

        // 根据摄像机的朝前向量和世界坐标系的向上向量，算出摄像机的朝右向量
        // 在世界坐标系中，无论摄像机的朝前向量怎样指向，它和世界坐标系的向上
        // 向量所构成的平面，摄像机的朝右向量必定垂直于它
        right_ = glm::normalize(glm::cross(forward_, world_up_));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        up_ = glm::normalize(glm::cross(right_, forward_));
    }
}