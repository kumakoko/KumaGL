// 动态改变顶点缓冲区数据的应用示例
#ifndef dynamic_vertex_buffer_app_h__
#define dynamic_vertex_buffer_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_source_texture.h"

class DynamicTextureApp : public kgl::App
{
public:
	DynamicTextureApp();
	virtual ~DynamicTextureApp();
	virtual void InitScene();
protected:
	virtual void RenderFrame();
private:
	kgl::GPUProgram*		gpu_program_;
	kgl::Primitive*			rectangle_primitive_;
	kgl::SourceTextureSPtr	texture_1_;
	GLfloat*				vertices_data_;
	float					last_update_vb_time_; // 上一次更新顶点缓冲数据的时间
};

#endif // dynamic_vertex_buffer_app_h__