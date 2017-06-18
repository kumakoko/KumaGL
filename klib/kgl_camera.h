// 摄像机类
#ifndef kgl_camera_h__
#define kgl_camera_h__

namespace kgl
{
    enum CameraType
    {
        ORTHO, FREE
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
		void ChangeHeading(float degrees);

		/// <summary>
		/// Move2s the d.
		/// </summary>
		/// <param name="x">The x.</param>
		/// <param name="y">The y.</param>
		void Move2D(int x, int y);

		/// <summary>
		/// Sets the mode.
		/// </summary>
		/// <param name="cam_mode">The cam_mode.</param>
		void SetMode(CameraType cam_mode);

		/// <summary>
		/// Sets the position.
		/// </summary>
		/// <param name="pos">The position.</param>
		void SetPosition(glm::vec3 pos);

		/// <summary>
		/// Sets the look at.
		/// </summary>
		/// <param name="pos">The position.</param>
		void SetLookAt(glm::vec3 pos);

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
		/// Initializes the view projection.
		/// </summary>
		/// <param name="cam_mod">The cam_mod.</param>
		/// <param name="pos">The position.</param>
		/// <param name="look_at">The look_at.</param>
		/// <param name="fov">The fov.</param>
		/// <param name="near_clip_distance">The near_clip_distance.</param>
		/// <param name="far_clip_distance">The far_clip_distance.</param>
		void InitViewProjection(CameraType cam_mod, const glm::vec3& pos, const glm::vec3& look_at, float fov, float near_clip_distance, float far_clip_distance);
       
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
		/// Gets the matricies.
		/// </summary>
		/// <param name="P">The p.</param>
		/// <param name="V">The v.</param>
		/// <param name="M">The m.</param>
		void GetMatricies(glm::mat4 &P, glm::mat4 &V, glm::mat4 &M);

		/// <summary>
		/// Gets the model view matrix.
		/// </summary>
		/// <returns>glm.mat4.</returns>
		glm::mat4 GetModelViewMatrix();

		/// <summary>
		/// Gets the projection matrix.
		/// </summary>
		/// <returns>const glm.mat4 &.</returns>
		inline const glm::mat4& GetProjectionMatrix() const
        {
            return projection_matrix_;
        }

		/// <summary>
		/// Gets the projection matrix.
		/// </summary>
		/// <returns>glm.mat4.</returns>
		inline glm::mat4 GetProjectionMatrix()
        {
            return projection_matrix_;
        }

		/// <summary>
		/// Gets the view matrix.
		/// </summary>
		/// <returns>glm.mat4.</returns>
		inline glm::mat4 GetViewMatrix()
        {
            return view_matrix_;
        }

		/// <summary>
		/// Gets the position.
		/// </summary>
		/// <returns>const glm.vec3 &.</returns>
		inline const glm::vec3& GetPosition() const
        {
            return camera_position_;
        }

		/// <summary>
		/// Gets the position.
		/// </summary>
		/// <returns>glm.vec3.</returns>
		inline glm::vec3 GetPosition()
        {
            return camera_position_;
        }

		/// <summary>
		/// Sets the camera speed.
		/// </summary>
		/// <param name="speed">The speed.</param>
		inline void SetCameraSpeed(float speed)
		{
			camera_speed_ = speed;
		}

    private:
        CameraType camera_mode_;

        int viewport_x_;
        int viewport_y_;

        int window_width_;
        int window_height;

        float aspect_;
        float field_of_view;
        float near_clip_;
        float far_clip_;

        float camera_speed_;
        float camera_yaw_;
        float camera_pitch_;

        float max_pitch_rate;
        float max_heading_rate;
        bool move_camera_;

        glm::vec3 camera_position_;
        glm::vec3 camera_position_delta_;
        glm::vec3 camera_look_at_;
        glm::vec3 camera_direction_;

        glm::vec3 camera_up_;
        glm::quat rotation_quaternion_;
        glm::vec3 mouse_position_;

        glm::mat4 projection_matrix_;
        glm::mat4 view_matrix_;
        glm::mat4 model_matrix_;
        glm::mat4 mvp_matrix_;

        bool is_dirty_;
    };

    typedef std::shared_ptr<Camera> CameraSPtr;
}


#endif // kgl_camera_h__