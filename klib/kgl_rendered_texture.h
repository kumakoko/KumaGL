// 可渲染的纹理
#ifndef kgl_rendered_texture_h__
#define kgl_rendered_texture_h__

#include "kgl_texture.h"

namespace kgl
{
    enum class RenderedTextureUsedType
    {
        COLOR_RGB_ONLY,
        COLOR_RGBA_ONLY,
        STENCIL_ONLY,
        DEPTH_ONLY,
        WITH_DEPTH_STENCIL,
    };

    class RenderedTexture : public Texture
    {
    public:
        //************************************
        // Method:    RenderedTexture
        // FullName:  kgl::RenderedTexture::RenderedTexture
        // Access:    public 
        // Returns:   
        // Qualifier:
        // Parameter: RenderedTextureUsedType used_type
        //************************************
        RenderedTexture(RenderedTextureUsedType used_type);

        //************************************
        // Method:    ~RenderedTexture
        // FullName:  kgl::RenderedTexture::~RenderedTexture
        // Access:    virtual public 
        // Returns:   
        // Qualifier:
        //************************************
        virtual ~RenderedTexture();

        //************************************
        // Method:    CreateFromFile
        // FullName:  kgl::Texture::CreateFromFile
        // Access:    virtual public 
        // Returns:   void
        // Qualifier:
        // Parameter: const std::string & file_name
        // Parameter: const TextureParams & texture_params
        //************************************
        virtual void CreateFromFile(const std::string& file_name, const TextureParams& texture_params);

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

        void Create(int32_t texture_width, int32_t texture_height);

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

    protected:
        RenderedTextureUsedType used_type_;
        GLuint                  fbo_;
        GLuint                  texture_id_;
        GLuint                  rbo_;
    };
}
#endif // kgl_rendered_texture_h__