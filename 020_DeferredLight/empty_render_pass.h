/*!
 * \file empty_render_pass.h
 * \date 2017/12/23 22:44
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef empty_render_pass_h__
#define empty_render_pass_h__

#include "../klib/kgl_gpu_program.h"

class EmptyRenderPass
{
public:
	EmptyRenderPass();
	~EmptyRenderPass();
	bool Init();
	void SetWVPMatrix(const glm::mat4& wvp_matrix);
	void Enable();
private:
	GLint wvp_matrix_loc_ = -1;
	kgl::GPUProgram* program_ = nullptr;
};

#endif // empty_render_pass_h__