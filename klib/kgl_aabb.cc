/**************************************************************************************************************************
Copyright(C) 2014-2018 www.xionggf.com

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute,sublicense, and/or sell copies of the Software, and to permit persons to whom the 
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
ARISING FROM,OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************************************/
#include "kgl_lib_pch.h"
#include "kgl_aabb.h"

namespace kgl
{
	AABB::AABB(const glm::vec3 &corner, float x, float y, float z) 
	{
		Set(corner, x, y, z);
	}

	AABB::AABB(void) 
	{
		corner_.x = 0; corner_.y = 0; corner_.z = 0;
		x_ = 1.0f;
		y_ = 1.0f;
		z_ = 1.0f;
	}

	AABB::~AABB() 
	{
	}

	void AABB::Set(const glm::vec3 &corner, float x, float y, float z) 
	{
		this->corner_ = corner;

		if (x < 0.0) {
			x = -x;
			this->corner_.x -= x;
		}
		if (y < 0.0) {
			y = -y;
			this->corner_.y -= y;
		}
		if (z < 0.0) {
			z = -z;
			this->corner_.z -= z;
		}
		this->x_ = x;
		this->y_ = y;
		this->z_ = z;
	}

	glm::vec3 AABB::getVertexP(const glm::vec3 &normal) const
	{
		glm::vec3 res = corner_;

		if (normal.x > 0)
			res.x += x_;

		if (normal.y > 0)
			res.y += y_;

		if (normal.z > 0)
			res.z += z_;

		return(res);
	}

	glm::vec3 AABB::getVertexN(const glm::vec3 &normal) const
	{

		glm::vec3 res = corner_;

		if (normal.x < 0)
			res.x += x_;

		if (normal.y < 0)
			res.y += y_;

		if (normal.z < 0)
			res.z += z_;

		return(res);
	}
}