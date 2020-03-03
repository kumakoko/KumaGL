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
/*!
 * \file kgl_md2_model.h
 * \date 2020/03/03 22:08
 *
 * \author www.xionggf.com
 * Contact: sun_of_lover@sina.com
 *
 * \brief 封装了读取MD2格式文件，显示MD2模型，播放动画的模块
 */
#ifndef kgl_md2_model_h__
#define kgl_md2_model_h__

#include "kgl_vertex_buffer_object.h"
#include "kgl_gpu_program.h"
#include "kgl_texture.h"
#include "kgl_md2_animation_status.h"

namespace kgl
{
	typedef float vec3_t[3];
	/** * @brief MD2文件中预计算的法线的数量  */
#define NUMVERTEXNORMALS 162
	// precalculated normal vectors
	//#define SHADEDOT_QUANT 16

	/** * @brief MD2文件中的魔数，为字符串"IDP2"，或者整数值844121161 */
#define MD2_IDENT (('2'<<24) + ('P'<<16) + ('D'<<8) + 'I')

	/** * @brief 模式的版本号 */
#define	MD2_VERSION 8

	/** * @brief 每个MD2模型中的顶点数 */
#define MAX_MD2_VERTS 2048

	// MD2文件的文件头格式结构体
	struct MD2FileHeader
	{
		int ident;			// 识别号，对应的字符串值为 "IDP2"
		int version;		// 版本号 8
		int skin_width;		// 模型皮肤纹理的宽度
		int skin_height;	// 模型皮肤纹理的高度
		int frame_size;		// 每帧的大小，以字节为单位
		int num_skins;		// 与模型相关联的皮肤数量
		int num_xyz;		// 模型的顶点数
		int num_st;			// 纹理坐标的数量
		int num_tris;		// 模型中三角形面片的数量
		int num_gl_cmds;    // OpenGL的命令数目
		int num_frames;		// 帧数目
		int ofs_skins;		// 纹理映射坐标数据在文件中的偏移量 (64 bytes each)
		int ofs_st;			// 纹理映射坐标数据在文件中的偏移量
		int ofs_tris;		// 三角形面片数据在文件中的偏移量
		int ofs_frames;		// 帧数据在文件中的偏移量
		int ofs_gl_cmds;    // OpenGL命令在文件中的偏移量
		int ofs_end;		// 文件结束位置在文件中的偏移量
	};

	// MD2文件中经过压缩的顶点结构体
	struct MD2CompressedVertex
	{
		unsigned char v[3];                // 经过压缩的顶点坐标的坐标值
		unsigned char lighting_normal_index;    // 一个索引值，指向本顶点对应具体的法线值，这个法线用来进行光照计算
	};

	// MD2文件中每一个动画帧的数据结构体，每一帧的顶点数据存储在这里
	struct MD2FrameData
	{
		float scale[3];					// 当前帧模型的缩放值
		float translate[3];				// 当前帧模型的平移值
		char name[16];					// 当前帧的名字
		MD2CompressedVertex verts[1];	// 本帧的第一个顶点的信息
	};

	// 每一个MD2 animation clip的信息
	struct MD2AnimClipInfo
	{
		int first_frame; // 本动画clip中第一帧的编号
		int last_frame;  // 本动画clip中最末帧的编号
		int fps;         // 每秒种本动画clip的帧数

		MD2AnimClipInfo() :first_frame(-1), last_frame(-1), fps(-1){}
		MD2AnimClipInfo(int f_frame, int l_frame, int f) :first_frame(f_frame), last_frame(l_frame), fps(f){}
	};

	typedef std::map<std::string, MD2AnimClipInfo> MD2AnimClipInfoDict;

	typedef std::map<std::string, MD2AnimClipInfoDict> MD2AnimClipInfoDictManager;

	class MD2Model
	{
	private:
		static MD2AnimClipInfoDictManager s_clip_info_dict_mgr;
	private:
		/*********************************************************
		根据给定的模型名，找到该模型对应的animation clip info dictionary
		@param const std::string & model_name 模型名
		@return 返回该模型对应的animation clip info dictionary的引用
		*********************************************************/
		static MD2AnimClipInfoDict& GetClipInfoDictFromManager(const std::string& model_name);

		/*********************************************************
		添加一个animation clip name - animation clip info的映射项到
		animation clip info dictionary里
		@param const std::string & clip_name animation clip的名字
		@param const MD2AnimClipInfo & clip_info animation clip的数据结构体
		@param MD2AnimClipInfoDict & dict animation clip info dictionary
		*********************************************************/
		static void AddClipInfoIntoDict(const std::string& clip_name, const MD2AnimClipInfo& clip_info, MD2AnimClipInfoDict& dict);

		/*********************************************************
		根据给定的animation clip name和clip info dictionary，找到对应的clip info结构体
		@param const std::string & animation_clip_name 待查找的clip name
		@param const MD2AnimClipInfoDict & dict 存储着clip info的dictionary
		@return 返回查找到的clip info结构体指针
		*********************************************************/
		static const MD2AnimClipInfo* GetClipInfoFromDictionary(const std::string& animation_clip_name, const MD2AnimClipInfoDict& dict);
	public:
		/*********************************************************
		构造函数
		@param GPUProgramSPtr shader 渲染本模型要用到的shader
		*********************************************************/
		MD2Model(GPUProgramSPtr shader);

		/*********************************************************
		析构函数
		*********************************************************/
		~MD2Model();

		/*********************************************************
		从文件file_path种装载本MD2模型
		@param          const char * file_path
		*********************************************************/
		void LoadFromFile(const char* file_path);

		/*********************************************************
		渲染本模型
		@param  const MD2AnimationStatus * anim_status
		@param  const glm::mat4 & model_matrix
		@param  const glm::mat4 & view_matrix
		@param  const glm::mat4 & projection_matrix
		*********************************************************/
		void Render(const MD2AnimationStatus* anim_status, const glm::mat4& model_matrix, const glm::mat4& view_matrix, const glm::mat4& projection_matrix);

		/*********************************************************
		开启名为animation_clip_name的animation
		@param const std::string & animation_clip_name 某个animation clip的名字
		@return
		*********************************************************/
		MD2AnimationStatus StartAnimation(const std::string& animation_clip_name);

		/*********************************************************

		@param const MD2AnimationStatus * anim_state
		@param float time_passed
		*********************************************************/
		void UpdateAnimation(MD2AnimationStatus* anim_state, float time_passed);

		void AddKeyframeInfo(const std::string& clip_name, int start_frame, int end_frame, int fps);

		void AddKeyframeInfo(const std::string& clip_name, const MD2AnimClipInfo& clip_info);

		static MD2AnimClipInfo	animlist[21];
	private:
		/** * @brief MD2文件头信息 */
		MD2FileHeader header;

		/** * @brief 二维数组，第一个索引记录这每一帧，第二个索引记录着某一帧中每个顶点的信息 */
		std::vector<std::vector<glm::vec3>> vertices_;

		/** * @brief 二维数组，第一个索引记录这每一帧，第二个索引记录着某一帧中每个顶点的法线在法线数组中的索引 */
		std::vector<std::vector<int>> normals_;

		/** * @brief 用来执行渲染的GL命令值 */
		std::vector<int> gl_command_;

		/** * @brief 每个关键帧所对应的vbo */
		std::vector<VertexBufferObject> frame_vertices_vbo_;

		/** * @brief 所有关键帧中用到的纹理映射坐标的vbo，每一帧的顶点的纹理映射坐标都是一样的 */
		VertexBufferObject vbo_texture_coords_;

		std::vector<int> render_modes_;

		/** * @brief 每一帧在渲染模型时，所要渲染的顶点数 */
		std::vector<int> render_vertices_num_;

		GPUProgramSPtr shader_;

		TextureSPtr skin_texture_;

		unsigned int vao_;

		const MD2AnimClipInfoDict* clip_info_dict_ = nullptr;

		std::string model_file_name_;
	};
}

#endif // kgl_md2_model_h__