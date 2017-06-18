// 纹理类的基类
#ifndef kgl_cubemap_texture_h__
#define kgl_cubemap_texture_h__

#include "kgl_texture.h"

namespace kgl
{
    class CubemapTexture : public Texture
    {
	public:
		/************************************************************
		 Method:    CubemapTexture
		 FullName:  kgl::CubemapTexture::CubemapTexture
		 Access:    public 
		 Returns:   
		 Parameter: const std::string & positive_x_file X方向上正方向贴图
		 Parameter: const std::string & negative_x_file X方向上负方向贴图
		 Parameter: const std::string & positive_y_file Y方向上正方向贴图
		 Parameter: const std::string & negative_y_file Y方向上负方向贴图
		 Parameter: const std::string & positive_z_file Z方向上正方向贴图
		 Parameter: const std::string & negative_z_file Z方向上负方向贴图
		 Details:
		************************************************************/
		CubemapTexture(
			const std::string& positive_x_file,
			const std::string& negative_x_file,
			const std::string& positive_y_file,
			const std::string& negative_y_file,
			const std::string& positive_z_file,
			const std::string& negative_z_file);

		/************************************************************
		 Method:    ~CubemapTexture
		 FullName:  kgl::CubemapTexture::~CubemapTexture
		 Access:    public 
		 Returns:   
		 Details:
		************************************************************/
		~CubemapTexture();

		/************************************************************
		 Method:    Load
		 FullName:  kgl::CubemapTexture::Load
		 Access:    public 
		 Returns:   bool
		 Qualifier:
		 Details:
		************************************************************/
		bool Load();

		/************************************************************
		 Method:    Bind
		 FullName:  kgl::CubemapTexture::Bind
		 Access:    public 
		 Returns:   void
		 Qualifier:
		 Parameter: GLenum TextureUnit
		 Details:
		************************************************************/
		void Bind(GLenum TextureUnit);

		//************************************
		// Method:    ActiveBind
		// FullName:  kgl::Texture::ActiveBind
		// Access:    virtual public 
		// Returns:   void
		// Qualifier: 激活并绑定本纹理到OpenGL管线
		// Parameter: GLuint slot_index
		//************************************
		virtual void ActiveBind(GLuint slot_index) override;
	private:
		std::string file_name_[6];
		GLuint texture_id_;
    };

	typedef std::shared_ptr<CubemapTexture> CubemapTextureSPtr;
}

#endif // kgl_cubemap_texture_h__