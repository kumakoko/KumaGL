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
#include "kgl_plane.h"

namespace kgl
{

	Plane::Plane(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3)
	{
		SetThreePoints(v1, v2, v3);
	}

	Plane::Plane() 
	{
	}

	Plane::~Plane() 
	{
	}

	void Plane::SetThreePoints(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3)
	{
		glm::vec3 aux1, aux2;
		aux1 = v1 - v2;
		aux2 = v3 - v2;
		normal_ = glm::normalize(aux2 * aux1);
		point_ = v2;
		d_ = -glm::dot(normal_, point_);
	}

	void Plane::SetNormalAndPoint(const glm::vec3 &normal, const glm::vec3 &point) 
	{
		normal_ = glm::normalize(normal);
		point_ = point;
		d_ = -glm::dot(normal_, point_);
	}

	void Plane::SetCoefficients(float a, float b, float c, float d)
	{
		// 设置面法线
		normal_.x = a;
		normal_.y = b;
		normal_.z = c;
		float l = glm::length(normal_); // 法线的长度
		normal_ = glm::normalize(normal_); // 规格化长度
		this->d_ = d / l; // 求出截距
	}

	float Plane::Distance(const glm::vec3 &p)
	{
		return d_ + glm::dot(normal_,p);
	}

	void Plane::InfoPrint()
	{
		// printf("Plane("); normal_.print(); printf("# %f)", d_);
	}

}