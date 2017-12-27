/*!
 * \file kgl_transform.h
 * \date 2017/12/21 10:40
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
#ifndef kgl_transform_h__
#define kgl_transform_h__

namespace kgl
{
	struct Orientation
	{
		glm::vec3 m_scale;
		glm::vec3 m_rotation;
		glm::vec3 m_pos;
		Orientation();
	};
}

#endif // kgl_transform_h__