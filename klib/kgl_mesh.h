// Mesh类
#ifndef kgl_mesh_h__
#define kgl_mesh_h__

#include "kgl_texture.h"
#include "kgl_vertex_type.h"
#include "kgl_gpu_program.h"

namespace kgl
{
    class Mesh : public boost::noncopyable
    {
    public:
        Mesh();
        virtual ~Mesh();

        //************************************
        // Method:    Setup
        // FullName:  kgl::Mesh::Setup
        // Access:    public 
        // Returns:   void
        // Qualifier:
        // Parameter: VertexType vertex_type
        // Parameter: const unsigned char * vertex_data
        // Parameter: uint32_t vertex_byte_count
        // Parameter: const uint16_t * index_data
        // Parameter: uint32_t index_byte_count
        // Parameter: const std::vector<TextureSPtr> & texture_array
        //************************************
        void Setup(VertexType vertex_type, const unsigned char* vertex_data, uint32_t vertex_byte_count,const uint32_t* index_data,uint32_t index_byte_count,const std::vector<TextureSPtr>& texture_array);
        
        //************************************
        // Method:    Draw
        // FullName:  kgl::Mesh::Draw
        // Access:    public 
        // Returns:   void
        // Qualifier:
        //************************************
        void Draw();

        inline void SetShader(GPUProgramSPtr shader)
        {
            shader_ = shader;
        }

        inline GPUProgramSPtr GetShader()
        {
            return shader_;
        }

    protected:
        //************************************
        // Method:    Clear
        // FullName:  kgl::Mesh::Clear
        // Access:    protected 
        // Returns:   void
        // Qualifier:
        //************************************
        void Clear();

        //************************************
        // Method:    SetupVertexAttributeBuffer
        // FullName:  kgl::Mesh::SetupVertexAttributeBuffer
        // Access:    protected 
        // Returns:   void
        // Qualifier:
        // Parameter: VertexType vertex_type
        //************************************
        void SetupVertexAttributeBuffer(VertexType vertex_type);

        //************************************
        // Method:    SetupVertexAttributeBufferP
        // FullName:  kgl::Mesh::SetupVertexAttributeBufferP
        // Access:    protected 
        // Returns:   void
        // Qualifier:
        //************************************
        void SetupVertexAttributeBufferP();

        //************************************
        // Method:    SetupVertexAttributeBufferPNTBT1
        // FullName:  kgl::Mesh::SetupVertexAttributeBufferPNTBT1
        // Access:    protected 
        // Returns:   void
        // Qualifier:
        //************************************
        void SetupVertexAttributeBufferPNTBT1();

        //************************************
        // Method:    SetupVertexAttributeBufferPNTBT2
        // FullName:  kgl::Mesh::SetupVertexAttributeBufferPNTBT2
        // Access:    protected 
        // Returns:   void
        // Qualifier:
        //************************************
        void SetupVertexAttributeBufferPNTBT2();

        //************************************
        // Method:    SetupVertexAttributeBufferPNTBT3
        // FullName:  kgl::Mesh::SetupVertexAttributeBufferPNTBT3
        // Access:    protected 
        // Returns:   void
        // Qualifier:
        //************************************
        void SetupVertexAttributeBufferPNTBT3();

        //************************************
        // Method:    SetupVertexAttributeBufferPNTBT4
        // FullName:  kgl::Mesh::SetupVertexAttributeBufferPNTBT4
        // Access:    protected 
        // Returns:   void
        // Qualifier:
        //************************************
        void SetupVertexAttributeBufferPNTBT4();

        //************************************
        // Method:    SetupVertexAttributeBufferPC
        // FullName:  kgl::Mesh::SetupVertexAttributeBufferPC
        // Access:    protected 
        // Returns:   void
        // Qualifier:
        //************************************
        void SetupVertexAttributeBufferPC();

        //************************************
        // Method:    SetupVertexAttributeBufferPN
        // FullName:  kgl::Mesh::SetupVertexAttributeBufferPN
        // Access:    protected 
        // Returns:   void
        // Qualifier:
        //************************************
        void SetupVertexAttributeBufferPN();

        //************************************
        // Method:    SetupVertexAttributeBufferPNTB
        // FullName:  kgl::Mesh::SetupVertexAttributeBufferPNTB
        // Access:    protected 
        // Returns:   void
        // Qualifier:
        //************************************
        void SetupVertexAttributeBufferPNTB();

        //************************************
        // Method:    SetupVertexAttributeBufferPNT1
        // FullName:  kgl::Mesh::SetupVertexAttributeBufferPNT1
        // Access:    protected 
        // Returns:   void
        // Qualifier:
        //************************************
        void SetupVertexAttributeBufferPNT1();

        //************************************
        // Method:    SetupVertexAttributeBufferPNT2
        // FullName:  kgl::Mesh::SetupVertexAttributeBufferPNT2
        // Access:    protected 
        // Returns:   void
        // Qualifier:
        //************************************
        void SetupVertexAttributeBufferPNT2();

        //************************************
        // Method:    SetupVertexAttributeBufferPNT3
        // FullName:  kgl::Mesh::SetupVertexAttributeBufferPNT3
        // Access:    protected 
        // Returns:   void
        // Qualifier:
        //************************************
        void SetupVertexAttributeBufferPNT3();

        //************************************
        // Method:    SetupVertexAttributeBufferPNT4
        // FullName:  kgl::Mesh::SetupVertexAttributeBufferPNT4
        // Access:    protected 
        // Returns:   void
        // Qualifier:
        //************************************
        void SetupVertexAttributeBufferPNT4();
    protected:
        uint8_t*                    vertex_data_;       // 顶点缓冲区首指针
        uint32_t                    vertex_byte_count_; // 顶点数据的字节数
        uint32_t*                   index_data_;
        uint32_t                    index_byte_count_;
        std::vector<TextureSPtr>    texture_array_;
        GLuint                      vao_;
        GLuint                      vbo_;
        GLuint                      ebo_;
        VertexType                  vertex_type_;
        GPUProgramSPtr              shader_;
    };
}
#endif // kgl_mesh_h__