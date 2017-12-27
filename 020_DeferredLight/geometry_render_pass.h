// 几何处理阶段，也是第一个执行阶段
#ifndef geometry_render_pass_h__
#define geometry_render_pass_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_gpu_program.h"

class GeometryRenderPass
{
public:
	void Init();
	void SetWVP(const glm::mat4& WVP);
	void SetWorldMatrix(const glm::mat4& WVP);
	void SetColorTextureUnit(unsigned int TextureUnit);
	void Enable();
private:
	kgl::GPUProgramSPtr gpu_program_;
	GLint wvp_matrix_loc_ = -1;
	GLint world_matrix_loc_ = -1;
	GLint color_texture_unit_loc_ = -1;
};

#endif // geometry_render_pass_h__
