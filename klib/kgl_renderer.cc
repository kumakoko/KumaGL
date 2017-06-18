// 渲染器类
#include "kgl_lib_pch.h"
#include "kgl_renderer.h"
#include "kgl_defines.h"

namespace kgl
{
    Renderer::Renderer()
    {
    }


    Renderer::~Renderer()
    {
    }

    void Renderer::Init(bool use_experimental)
    {
        glewExperimental = use_experimental ? GL_TRUE : GL_FALSE;
        GLenum r = glewInit();
        
        if (r != GLEW_OK)
        {
            // std::cout << "Failed to initialize GLEW" << std::endl;
            return;
        }
        
        glGetError();
    }

    void Renderer::Clear()
    {
        std::vector<std::string> error_desc_array;
        std::vector<GLenum> error_code_array;
        glClearColor(0.35f, 0.53f, 0.7f, 1.0f);
        // glClearColor(0.f, 0.f, 0.f, 1.0f);
        // THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // THROW_GL_EXCEPTION(error_desc_array, error_code_array, __FILE__, __LINE__);
    }
}