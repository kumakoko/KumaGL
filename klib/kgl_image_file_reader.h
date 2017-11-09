// 图像读取器
#ifndef kgl_image_file_reader_h__
#define kgl_image_file_reader_h__

namespace kgl
{
	class GLBitmap
	{
	private:
		int				width_ = 0;
		int             height_ = 0;
		GLuint			rgb_mode_ = GL_RGB;
		unsigned char*	buffer_ = nullptr;

	public:
		GLBitmap()
		{
			width_ = 0;
			height_ = 0;
			rgb_mode_ = GL_RGB;
			buffer_ = nullptr;
		}

		GLBitmap(int width, int height, GLuint rgb_mode)
		{
			width_ = width;
			height_ = height;
			rgb_mode_ = rgb_mode;

			switch (rgb_mode)
			{
			default:
			case GL_RGB:
				buffer_ = new unsigned char[width_*height_ * 3];
				break;
			case GL_RGBA:
				buffer_ = new unsigned char[width_*height_ * 4];
				break;
			}
		}

		inline bool IsInvalid() const
		{
			return nullptr == buffer_;
		}

		inline int width()const{ return width_; }
		inline int height()const { return height_; }
		
		inline const unsigned char* Buffer() const
		{
			return buffer_;
		}
		inline GLuint RgbMode()const { return rgb_mode_; }

		void WriteRGB(int first_pixel_component_index, unsigned char r, unsigned char g, unsigned char b);

		void WriteRGBA(int first_pixel_component_index, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	};

	typedef std::shared_ptr<GLBitmap> GLBitmapSPtr;

	class ImageFileReader
	{
	public:
		ImageFileReader();
		~ImageFileReader();
		GLBitmapSPtr GetGLBitmapFromFile(const std::string& file_name, GLuint desired_rgb_mode);
	};
}

#endif // kgl_image_file_reader_h__