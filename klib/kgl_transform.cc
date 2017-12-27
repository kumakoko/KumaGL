#include "kgl_lib_pch.h"
#include "kgl_transform.h"

namespace kgl
{
	Orientation::Orientation()
	{
		m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
		m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		m_pos = glm::vec3(0.0f, 0.0f, 0.0f);
	}
}