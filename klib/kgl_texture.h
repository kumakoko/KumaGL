// 纹理类的基类
#ifndef kgl_texture_h__
#define kgl_texture_h__

namespace kgl
{
	enum TextureType
	{
		NONE,
		SOURCE_2D_TEXTURE,
		CUBEMAP_TEXTURE
	};

	struct TextureParams
	{
		GLint WrapSMode; //  GL_REPEAT
		GLint WrapTMode; //  GL_REPEAT
		GLint MinFilterMode;// GL_TEXTURE_MIN_FILTER
		GLint MagFilterMode;
		int   LoadChannel;		//  SOIL_LOAD_RGB;
		GLint InternalFormat;   // 纹理在内存（系统或者显卡中）中的存储模式 GL_RGB
		GLint SrcImgFormat;   // 原始图片中的颜色数据的格式，这个值依赖于loadChannel，要和LoadChannel一一对应 GL_RGB
		GLint SrcImgPixelComponentType; // 原始图片中颜色每一个分量在内存中占用的字节类型 GL_UNSIGNED_BYTE
		bool  UseMipmap;
	};

	class Texture
	{
	public:
		Texture();
		virtual ~Texture();

		//************************************
		// Method:    CreateFromFile
		// FullName:  kgl::Texture::CreateFromFile
		// Access:    virtual public 
		// Returns:   void
		// Qualifier:
		// Parameter: const std::string & file_name
		// Parameter: const TextureParams & texture_params
		//************************************
		virtual void CreateFromFile(const std::string& file_name,const TextureParams& texture_params);

		//************************************
		// Method:    Create
		// FullName:  kgl::Texture::Create
		// Access:    virtual public 
		// Returns:   void
		// Qualifier:
		// Parameter: int width
		// Parameter: int height
		// Parameter: const TextureParams & texture_params
		//************************************
		virtual void Create(int width, int height, const TextureParams& texture_params);

		//************************************
		// Method:    GetWidth
		// FullName:  kgl::Texture::GetWidth
		// Access:    virtual public 
		// Returns:   int
		// Qualifier: const
		//************************************
		virtual int GetWidth() const;

		//************************************
		// Method:    GetHeight
		// FullName:  kgl::Texture::GetHeight
		// Access:    virtual public 
		// Returns:   int
		// Qualifier: const
		//************************************
		virtual int GetHeight() const; 

		//************************************
		// Method:    IsMipmap
		// FullName:  kgl::Texture::IsMipmap
		// Access:    virtual public 
		// Returns:   bool
		// Qualifier: const
		//************************************
		virtual bool IsMipmap() const;

		//************************************
		// Method:    ActiveBind
		// FullName:  kgl::Texture::ActiveBind
		// Access:    virtual public 
		// Returns:   void
		// Qualifier: 激活并绑定本纹理到OpenGL管线
		// Parameter: GLuint slot_index
		//************************************
		virtual void ActiveBind(GLuint slot_index);

		inline TextureType GetType()
		{
			return type_;
		}

	protected:
		TextureType type_;
	};

	typedef std::shared_ptr<Texture> TextureSPtr;
}

#endif // kgl_texture_h__