/**************************************************************************************************************************
Copyright(C) 2014-2019 www.xionggf.com

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
#include "kgl_lib_pch.h"
#include "kgl_md2_model.h"
#include "kgl_texture_manager.h"
#include "kgl_source_texture.h"
#include "kgl_string_convertor.h"
#include "kgl_error.h"

namespace kgl
{
	MD2AnimClipInfo MD2Model::animlist[21] =
	{
		// first, last, fps
		{ 0, 39, 9 },	// STAND
		{ 40, 45, 10 },	// RUN
		{ 46, 53, 10 },	// ATTACK
		{ 54, 57, 7 },	// PAIN_A
		{ 58, 61, 7 },	// PAIN_B
		{ 62, 65, 7 },	// PAIN_C
		{ 66, 71, 7 },	// JUMP
		{ 72, 83, 7 },	// FLIP
		{ 84, 94, 7 },	// SALUTE
		{ 95, 111, 10 },	// FALLBACK
		{ 112, 122, 7 },	// WAVE
		{ 123, 134, 6 },	// POINTIING
		{ 135, 153, 10 },	// CROUCH_STAND
		{ 154, 159, 7 },	// CROUCH_WALK
		{ 160, 168, 10 },	// CROUCH_ATTACK
		{ 196, 172, 7 },	// CROUCH_PAIN
		{ 173, 177, 5 },	// CROUCH_DEATH
		{ 178, 183, 7 },	// DEATH_FALLBACK
		{ 184, 189, 7 },	// DEATH_FALLFORWARD
		{ 190, 197, 7 },	// DEATH_FALLBACKSLOW
		{ 198, 198, 5 },	// BOOM
	};

#pragma warning( once : 4305 )

	MD2AnimClipInfoDictManager MD2Model::s_clip_info_dict_mgr;

	MD2AnimClipInfoDict& MD2Model::GetClipInfoDictFromManager(const std::string& model_name)
	{
		auto iter = s_clip_info_dict_mgr.find(model_name);

		if (s_clip_info_dict_mgr.end() == iter)
		{
			s_clip_info_dict_mgr.emplace(model_name, MD2AnimClipInfoDict());
			return s_clip_info_dict_mgr[model_name];
		}

		return iter->second;
	}

	void MD2Model::AddClipInfoIntoDict(const std::string& clip_name, const MD2AnimClipInfo& clip_info, MD2AnimClipInfoDict& dict)
	{
		dict.emplace(clip_name, clip_info);
	}

	const MD2AnimClipInfo* MD2Model::GetClipInfoFromDictionary(const std::string& animation_clip_name, const MD2AnimClipInfoDict& dict)
	{
		auto iter = dict.find(animation_clip_name);

		if (dict.end() == iter)
			return nullptr;

		return &(iter->second);
	}

	MD2Model::MD2Model(GPUProgramSPtr shader) :shader_(shader)
	{
	}

	MD2Model::~MD2Model()
	{
		shader_.reset();
		skin_texture_.reset();
	}

	void MD2Model::AddKeyframeInfo(const std::string& clip_name, int start_frame, int end_frame, int fps)
	{
		AddKeyframeInfo(clip_name, MD2AnimClipInfo(start_frame, end_frame, fps));
	}

	void MD2Model::AddKeyframeInfo(const std::string& clip_name, const MD2AnimClipInfo& clip_info)
	{
		MD2AnimClipInfoDict& dict = GetClipInfoDictFromManager(model_file_name_);

		//if (dict.size() == 0)
			AddClipInfoIntoDict(clip_name, clip_info, dict);
	}

	void MD2Model::LoadFromFile(const char* file_path)
	{
		model_file_name_ = file_path;

		FILE* fp = fopen(file_path, "rb");

		fread(&header, sizeof(MD2FileHeader), 1, fp); // 读取出MD2的文件头，拿到对应的信息

		// 开辟一个新空间，帧数乘以每帧的数据大小
		char* buffer = new char[header.num_frames * header.frame_size];
		fseek(fp, header.ofs_frames, SEEK_SET);
		fread(buffer, sizeof(char), header.num_frames * header.frame_size, fp);

		vertices_.resize(header.num_frames, std::vector<glm::vec3>(header.num_xyz)); 
		normals_.resize(header.num_frames, std::vector<int>(header.num_xyz));

		for (int i = 0; i < header.num_frames; ++i)
		{
			MD2FrameData* frame_ptr = (MD2FrameData*)&buffer[header.frame_size * i];

			for (int j = 0; j < header.num_xyz; ++j)
			{
				vertices_[i][j].x = frame_ptr->translate[0] + static_cast<float>(frame_ptr->verts[j].v[0]) * frame_ptr->scale[0];
				vertices_[i][j].y = frame_ptr->translate[1] + static_cast<float>(frame_ptr->verts[j].v[1]) * frame_ptr->scale[1];
				vertices_[i][j].z = frame_ptr->translate[2] + static_cast<float>(frame_ptr->verts[j].v[2]) * frame_ptr->scale[2];
				normals_[i][j] = frame_ptr->verts[j].lighting_normal_index;
			}
		}

		// 读取OpenGL的渲染命令
		gl_command_.resize(header.num_gl_cmds);
		fseek(fp, header.ofs_gl_cmds, SEEK_SET);		// 直接跳转到OpenGL命令在文件中的偏移量
		fread(gl_command_.data(), sizeof(int), header.num_gl_cmds, fp);

		// 创建用来放顶点的位置数据的vbo
		// 一个md2模型中，有多少关键帧，就有对应多少个的vbo
		frame_vertices_vbo_.resize(header.num_frames);

		for (int m = 0; m < header.num_frames; ++m)
		{
			VertexBufferObject& vbo = frame_vertices_vbo_[m];
			vbo.Create();
		}

		std::ptrdiff_t i = 0;
		int total_vertices = 0;

		// 创建用来放顶点的纹理贴图坐标的vbo
		vbo_texture_coords_.Create();

		while (1)
		{
			int* glcmd_pointer = gl_command_.data();

			// 第0个gl command，存储了action，action值如果为负数，表示使用三角形扇作为渲染指令
			// 为正数使用三角形条带作为渲染指令。action值的绝对值就是顶点的个数
			int action = *(glcmd_pointer + i); // gl_command_[i];

			if (action == 0)
				break;

			// action小于0，就用要么用三角形扇，要么用三角形条带
			int render_mode = action < 0 ? GL_TRIANGLE_FAN : GL_TRIANGLE_STRIP; // Extract rendering mode			
			int vertices_num = std::abs(action);

			i++;

			render_modes_.emplace_back(render_mode); // Remember the values
			render_vertices_num_.emplace_back(vertices_num);

			// 计算当前处理的这个图元中，每个顶点的纹理映射坐标
			for (int j = 0; j < vertices_num; ++j)
			{
				// 第1，2个gl command，就是该图元的第0个顶点的纹理映射坐标，
				// 第3个gl command，记录了该顶点在各关键帧的顶点数组（vertices_）中的索引值
				// 第4，5个gl command就是该图元的第1个顶点的纹理映射坐标，以此类推
				float* st_pointer_s = reinterpret_cast<float*>((glcmd_pointer + (i++)));
				float* st_pointer_t = reinterpret_cast<float*>((glcmd_pointer + (i++)));
				float s = *st_pointer_s;
				float t = *st_pointer_t;
				t = 1.0f - t; // 贴图坐标值上下翻转
				int vi = gl_command_[i++];

				vbo_texture_coords_.AddData(&s, 4); // Add texture coords to VBO
				vbo_texture_coords_.AddData(&t, 4);

				// vi就是每一个顶点，在vertices_数组中的索引值
				for (int k = 0; k < header.num_frames; ++k)
				{
					frame_vertices_vbo_[k].AddData(&vertices_[k][vi], 12); // 把从MD2的文件中读到的数据加入到VBO对象中
					frame_vertices_vbo_[k].AddData(&glm::vec3(1.0f,0.0f,0.0f), 12); // 找到了法线数据再行添加
				}
			}
		}

		// vertex buffer object创建好了，VAO也要接着创建
		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		for (int i = 0; i < header.num_frames; ++i)
		{
			frame_vertices_vbo_[i].Bind();
			frame_vertices_vbo_[i].UploadDataToGPU(GL_STATIC_DRAW);
		}

		frame_vertices_vbo_[0].Bind(); // Vertex and normals data parameters

		/* 顶点布局
		|0: position|1: texture coord|2:normal|3: next frame position|4: next frame texture coord|5: next frame normal
		*/

		// 顶点的位置坐标，下面的vao排列顺序和026_md2_keyframe_anim_vs.glsl中定义的顶点layout是一致的
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), 0);
		glEnableVertexAttribArray(3); // 下一帧的位置数据
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), 0);

		// 顶点的法线
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)(sizeof(glm::vec3)));
		glEnableVertexAttribArray(4); // Normals for next keyframe, now we can set it to same VBO
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)(sizeof(glm::vec3)));

		// 顶点的纹理映射坐标
		vbo_texture_coords_.Bind();
		vbo_texture_coords_.UploadDataToGPU(GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);

		bfs::path original_texture_path(file_path);

		// 约定md2文件和它所包含的texture文件在同一个目录，且用到的纹理和md2文件名本身同名
		// 拿到md2所在的目录的名字，目录后面是不带/号的
		bfs::path md2_file_path(file_path);
		bfs::path texture_path = md2_file_path.parent_path();
		texture_path /= md2_file_path.stem(); // 经过这些操作之后 如果file_path为 aa/bb/cc.md2，则texture_path为aa/bb/cc

		const char* texture_extensions[] = { ".jpg", ".jpeg", ".png", ".bmp", ".tga" };

		// 判断文件是否存在
		bool is_texture_file_exist = false;
		std::string texture_full_path;

		for (int i = 0; i < 5; ++i)
		{
			texture_full_path.clear();
			texture_full_path = texture_path.string();
			texture_full_path.append(texture_extensions[i]);

			is_texture_file_exist = bfs::exists(bfs::path(texture_full_path));

			if (is_texture_file_exist)
			{
				break;
			}
		}

		if (is_texture_file_exist)
		{
			skin_texture_ = std::make_shared<kgl::SourceTexture>();

			kgl::TextureParams texture_param;
			texture_param.wrap_s_mode = GL_REPEAT;
			texture_param.wrap_t_mode = GL_REPEAT;
			texture_param.mag_filter_mode = GL_LINEAR;
			texture_param.min_filter_mode = GL_LINEAR;
			texture_param.internal_format = GL_RGB;
			texture_param.src_img_px_component_type = GL_UNSIGNED_BYTE;
			texture_param.src_img_format = GL_RGB;
			texture_param.used_mipmap = false;
			skin_texture_->CreateFromFile(texture_full_path, texture_param);
		}

		fclose(fp);
	}

	void MD2Model::Render(const MD2AnimationStatus* anim_status, const glm::mat4& model_matrix, const glm::mat4& view_matrix, const glm::mat4& projection_matrix)
	{
		glm::mat4 normal_matrix = glm::transpose(glm::inverse(model_matrix));

		shader_->Use();
		shader_->ApplyMatrix(glm::value_ptr(projection_matrix), "u_projection_matrix");
		shader_->ApplyMatrix(glm::value_ptr(view_matrix), "u_view_matrix");
		shader_->ApplyMatrix(glm::value_ptr(model_matrix), "u_model_matrix");
		shader_->ApplyMatrix(glm::value_ptr(normal_matrix), "u_normal_matrix");

		glBindVertexArray(vao_);

		int total_offset = 0;

		if (skin_texture_)
		{
			shader_->ApplyTexture(skin_texture_, "model_texture", 0);
		}

		if (anim_status == NULL)
		{
			glEnableVertexAttribArray(0);
			frame_vertices_vbo_[0].Bind();
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), 0);

			shader_->ApplyFloat(-1.0f, "u_interpolation");

			for (std::size_t i = 0; i < render_modes_.size(); ++i)
			{
				glDrawArrays(render_modes_[i], total_offset, render_vertices_num_[i]);
				total_offset += render_vertices_num_[i];
			}
		}
		else
		{
			// 设置当前帧
			glEnableVertexAttribArray(0);
			frame_vertices_vbo_[anim_status->current_frame()].Bind();
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), 0);

			glEnableVertexAttribArray(3);
			frame_vertices_vbo_[anim_status->next_frame()].Bind();
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), 0);

			glEnableVertexAttribArray(2);
			frame_vertices_vbo_[anim_status->current_frame()].Bind();
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), 0);

			glEnableVertexAttribArray(4);
			frame_vertices_vbo_[anim_status->next_frame()].Bind();
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), 0);

			shader_->ApplyFloat(anim_status->interpolation_percent(), "u_interpolation");

			for (std::size_t i = 0; i < render_modes_.size(); ++i)
			{
				glDrawArrays(render_modes_[i], total_offset, render_vertices_num_[i]);
				total_offset += render_vertices_num_[i];
			}		
		}
	}

	MD2AnimationStatus MD2Model::StartAnimation(const std::string& animation_clip_name)
	{
		const MD2AnimClipInfoDict& clip_dict = GetClipInfoDictFromManager(model_file_name_);
		const MD2AnimClipInfo* info = GetClipInfoFromDictionary(animation_clip_name, clip_dict);

		MD2AnimationStatus res;

		if (nullptr == info)
		{
			return res;
		}

		res.set_start_frame(info->first_frame);
		res.set_end_frame(info->last_frame);
		res.set_current_frame(info->first_frame);
		res.set_next_frame(info->first_frame + 1);
		res.set_fps(info->fps);
		res.set_type(animation_clip_name.c_str());
		res.set_current_time(0.0f);
		res.set_old_time(0.0f);
		res.set_interpolation_percent(0.0f);
		return res;
	}

	void MD2Model::UpdateAnimation(MD2AnimationStatus* anim_state, float time_passed)
	{
		anim_state->set_current_time(anim_state->current_time() + time_passed);

		if (anim_state->IsShouldUseNextFrame())
		{
			anim_state->set_old_time(anim_state->current_time());
			anim_state->set_current_frame(anim_state->next_frame());
			anim_state->set_next_frame(anim_state->next_frame() + 1);

			if (anim_state->next_frame() > anim_state->end_frame())
				anim_state->set_next_frame(anim_state->start_frame());
		}

		anim_state->set_interpolation_percent(float(anim_state->fps()) * (anim_state->current_time() - anim_state->old_time()));
	}

}