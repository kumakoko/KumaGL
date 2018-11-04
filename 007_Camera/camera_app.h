// 动态改变顶点缓冲区数据的应用示例
#ifndef camera_app_h__
#define camera_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_source_texture.h"

class CameraApp : public kgl::App
{
public:
    CameraApp();
    virtual ~CameraApp();
    virtual void InitScene();
protected:
    virtual void RenderScene();
    virtual void OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode);
    virtual void OnMouseAction(GLFWwindow* window, double xpos, double ypos);
    virtual void OnScrollAction(GLFWwindow* window, double xoffset, double yoffset);
    virtual void ProcessInput();
private:
    kgl::GPUProgram*        gpu_program_;
    kgl::Primitive*         rectangle_primitive_;
    kgl::SourceTextureSPtr  texture_1_;
    kgl::SourceTextureSPtr  texture_2_;
};

#endif // camera_app_h__