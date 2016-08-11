// 展示一个简单的三角形
#ifndef triangle_app_h__
#define triangle_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"

class TriangleApp : public kgl::App
{
public:
	TriangleApp();
	virtual ~TriangleApp();
	virtual void InitScene();
protected:
	virtual void RenderFrame();
private:
	kgl::GPUProgram* gpu_program_;
	kgl::Primitive*  rectangle_primitive_;
	kgl::Primitive*  triangle_primitive_1_;
	kgl::Primitive*  triangle_primitive_2_;
	kgl::Primitive*  triangle_primitive_3_;
	kgl::Primitive*  triangle_primitive_4_;
};


#endif // triangle_app_h__