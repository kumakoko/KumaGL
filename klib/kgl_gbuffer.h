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
		
		/// <summary>
		/// G-buffer所视同的纹理种类，表明某G-buffer是用来存储什么数据
		/// 并且被bind为哪个texture unit
		/// </summary>
		enum GBUFFER_TEXTURE_TYPE
		{
			/// <summary>
			/// 存储了fragment的position的G-buffer，对应于texture unit 0
			/// </summary>
			GBUFFER_TEXTURE_TYPE_POSITION,

			/// <summary>
			/// 存储了fragment的position的G-buffer，对应于texture unit 1
			/// </summary>
			GBUFFER_TEXTURE_TYPE_DIFFUSE,

			/// <summary>
			/// 存储了fragment的position的G-buffer，对应于texture unit 2
			/// </summary>
			GBUFFER_TEXTURE_TYPE_NORMAL,

			/// <summary>
			/// G-buffer缓冲区的个数，本项一定是排在最后面
			/// </summary>
			GBUFFER_NUM_TEXTURES
		};

		/// <summary>
		/// <see cref="GBuffer"/> 类的构造函数
		/// </summary>
		GBuffer();

		/// <summary>
		/// Finalizes an instance of the <see cref="GBuffer"/> 类的析构函数
		/// </summary>
		~GBuffer();
		
		/// <summary>
		/// 根据给定的窗口高宽，初始化G-buffer对象及其对应的缓冲区
		/// </summary>
		/// <param name="window_width">窗口宽度</param>
		/// <param name="window_height">窗口高度</param>
		/// <returns>创建成功返回true，失败返回false</returns>
		void Initialise(GLuint window_width, GLuint window_height);
		/*
		/// <summary>
		/// Starts the frame.
		/// </summary>
		void StartFrame();

		/// <summary>
		/// Binds for geom pass.
		/// </summary>
		void BindForGeomPass();

		/// <summary>
		/// Binds for stencil pass.
		/// </summary>
		void BindForStencilPass();

		/// <summary>
		/// Binds for light pass.
		/// </summary>
		void BindForLightPass();

		/// <summary>
		/// Binds for final pass.
		/// </summary>
		void BindForFinalPass();
		*/
		void StartGeometryRenderPass();

		void StartLightingRenderPass();

		void Blit();

		void EndPass();
	private:
		/// <summary>
		/// frame buffer object id
		/// </summary>
		GLuint fbo_;

		GLuint position_info_texture_ = 0;

		GLuint normal_info_texture_ = 0;

		GLuint albedo_info_texture_ = 0;

		GLuint window_width_ = 0;
		GLuint window_height_ = 0;

		/// <summary>
		/// The texture_buffers_
		/// </summary>
//		GLuint texture_buffers_[GBUFFER_NUM_TEXTURES];
		
		/// <summary>
		/// The texture_buffers_
		/// </summary>
		GLuint depth_texture_;

		/// <summary>
		/// The final_texture_
		/// </summary>
		GLuint final_texture_;
		
	};
}

#endif // kgl_gbuffer_h__
