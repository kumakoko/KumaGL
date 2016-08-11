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
		Camera();
		~Camera();

		void Reset();
		
		void Update();

		void Move(CameraDirection dir);

		void ChangePitch(float degrees);

		void ChangeHeading(float degrees);

		void Move2D(int x, int y);

		void SetMode(CameraType cam_mode);

		void SetPosition(glm::vec3 pos);

		void SetLookAt(glm::vec3 pos);

		void SetFOV(float fov);

		void SetViewport(int loc_x, int loc_y, int width, int height);

		void SetClipping(float near_clip_distance, float far_clip_distance);

		inline void InitViewProjection(CameraType cam_mod, const glm::vec3& pos, const glm::vec3& look_at, float fov, float near_clip_distance, float far_clip_distance)
		{
			this->SetMode(cam_mod);
			this->SetPosition(pos);
			this->SetLookAt(look_at);
			this->SetFOV(fov);
			this->SetClipping(near_clip_distance, far_clip_distance);
		}

		inline CameraType GetMode() const
		{
			return camera_mode_;
		}

		void GetViewport(int &loc_x, int &loc_y, int &width, int &height);

		void GetMatricies(glm::mat4 &P, glm::mat4 &V, glm::mat4 &M);

		const glm::mat4& GetProjectionMatrix() const
		{
			return projection_matrix_;
		}

		glm::mat4 GetProjectionMatrix()
		{
			return projection_matrix_;
		}

		glm::mat4 GetViewMatrix() 
		{
			return view_matrix_;
		}

		const glm::vec3& GetPosition() const
		{
			return camera_position_;
		}

		glm::vec3 GetPosition()
		{
			return camera_position_;
		}

	private:
		CameraType camera_mode_;

		int viewport_x;
		int viewport_y;

		int window_width;
		int window_height;

		float aspect_;
		float field_of_view;
		float near_clip_;
		float far_clip_;

		float camera_scale;
		float camera_yaw_;
		float camera_pitch_;

		float max_pitch_rate;
		float max_heading_rate;
		bool move_camera;

		glm::vec3 camera_position_;
		glm::vec3 camera_position_delta_;
		glm::vec3 camera_look_at_;
		glm::vec3 camera_direction_;

		glm::vec3 camera_up_;
		glm::quat rotation_quaternion;
		glm::vec3 mouse_position;

		glm::mat4 projection_matrix_;
		glm::mat4 view_matrix_;
		glm::mat4 model;
		glm::mat4 MVP;

		bool is_dirty_;
	};

	typedef std::shared_ptr<Camera> CameraPtr;
}


#endif // kgl_camera_h__