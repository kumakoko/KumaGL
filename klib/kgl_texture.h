// 纹理类的基类
#ifndef kgl_texture_h__
#define kgl_texture_h__

namespace kgl
{
	/// <summary>
	/// 纹理的类型
	/// </summary>
	enum TextureType
    {
        NONE,

		/// <summary>
		/// 2D纹理
		/// </summary>
		SOURCE_2D_TEXTURE,

		/// <summary>
		/// 立方体纹理
		/// </summary>
		CUBEMAP_TEXTURE,

		/// <summary>
		/// 可作为渲染目标的纹理
		/// </summary>
		RENDERED_TEXTURE
    };

    struct TextureParams
    {
		/// <summary>
		/// The wrap_s_mode
		/// </summary>
		GLint wrap_s_mode;

		/// <summary>
		/// The wrap_t_mode
		/// </summary>
		GLint wrap_t_mode;

		/// <summary>
		/// The min_filter_mode
		/// </summary>
		GLint mag_filter_mode;

		/// <summary>
		/// The mag_filter_mode
		/// </summary>
		GLint min_filter_mode;

		/// <summary>
		/// The load_channel
		/// </summary>
		int load_channel;

		/// <summary>
		/// 纹理在内存（系统或者显卡中）中的存储模式 
		/// </summary>
		GLint internal_format;

		/// <summary>
		/// 原始图片中的颜色数据的格式，这个值依赖于loadChannel，要和LoadChannel一一对应
		/// </summary>
		GLint src_img_format;
		
		/// <summary>
		/// 原始图片中颜色每一个分量在内存中占用的字节类型
		/// </summary>
		GLint src_img_px_component_type;
		
		/// <summary>
		/// The is_use_mipmap
		/// </summary>
		bool  used_mipmap;
    };

	/// <summary>
	/// 纹理类的基类
	/// </summary>
	class Texture
    {
    public:
		/// <summary>
		/// Initializes a new instance of the <see cref="Texture"/> class.
		/// </summary>
		Texture();

		/// <summary>
		/// Finalizes an instance of the <see cref="Texture"/> class.
		/// </summary>
		virtual ~Texture();

		/// <summary>
		/// Creates from file.
		/// </summary>
		/// <param name="file_name">The file_name.</param>
		/// <param name="texture_params">The texture_params.</param>
		virtual void CreateFromFile(const std::string& file_name, const TextureParams& texture_params);

		/// <summary>
		/// Creates the specified width.
		/// </summary>
		/// <param name="width">The width.</param>
		/// <param name="height">The height.</param>
		/// <param name="texture_params">The texture_params.</param>
		virtual void Create(int width, int height, const TextureParams& texture_params);

		/// <summary>
		/// Gets the width.
		/// </summary>
		/// <returns>int.</returns>
		virtual int GetWidth() const;

		/// <summary>
		/// Gets the height.
		/// </summary>
		/// <returns>int.</returns>
		virtual int GetHeight() const;

		/// <summary>
		/// Determines whether this instance is mipmap.
		/// </summary>
		/// <returns>bool.</returns>
		virtual bool IsMipmap() const;

		/// <summary>
		///  激活并绑定本纹理到OpenGL管线
		/// </summary>
		/// <param name="slot_index">The slot_index.</param>
		virtual void ActiveBind(GLuint slot_index);

		/// <summary>
		/// Gets the type.
		/// </summary>
		/// <returns>kgl.TextureType.</returns>
		inline TextureType GetType()
        {
            return type_;
        }

    protected:
		/// <summary>
		/// 纹理的类型
		/// </summary>
		TextureType type_;
    };

    typedef std::shared_ptr<Texture> TextureSPtr;
}

#endif // kgl_texture_h__