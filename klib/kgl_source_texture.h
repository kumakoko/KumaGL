// 纹理类的基类
#ifndef kgl_source_texture_h__
#define kgl_source_texture_h__

#include "kgl_texture.h"

namespace kgl
{
	class SourceTexture : public Texture
	{
	public:
		SourceTexture();
		virtual ~SourceTexture();
		virtual void CreateFromFile(const std::string& file_name, const TextureParams& texture_params);
		virtual void Create(int width, int height, const TextureParams& texture_params);
		virtual int GetWidth() const;
		virtual int GetHeight() const;
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

		virtual void Update(GLint x_offset, GLint y_offset, GLsizei width, GLsizei height, const GLvoid* data);
	protected:
		TextureParams params_;
		GLuint texture_id_;
		int width_;
		int height_;
	};

	typedef std::shared_ptr<SourceTexture> SourceTextureSPtr;
}

#endif // kgl_source_texture_h__