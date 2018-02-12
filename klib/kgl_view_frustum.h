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
/*!
 * \file kgl_view_frustum.h
 * \date 2018/02/12 11:48
 *
 * \author www.xionggf.com
 * Contact: sun_of_lover@sina.com
 *
 * \brief 
 *
 * TODO: 视截体类
 *
 * \note
*/
#ifndef kgl_view_frustum_h__
#define kgl_view_frustum_h__

namespace kgl
{
	class Plane;
	class AABB;

	class ViewFrustum
	{
	private:
		enum
		{
			TOP = 0,
			BOTTOM,
			LEFT,
			RIGHT,
			NEARP,
			FARP
		};

	public:
		enum Intersection
		{
			OUTSIDE, 
			INTERSECT,
			INSIDE
		};

		Plane pl[6];


		glm::vec3 ntl;
		glm::vec3 ntr;
		glm::vec3 nbl;
		glm::vec3 nbr;
		glm::vec3 ftl;
		glm::vec3 ftr;
		glm::vec3 fbl;
		glm::vec3 fbr;
		float nearD, farD, ratio, angle, tang;
		float nw, nh, fw, fh;

		ViewFrustum();
		~ViewFrustum();

		void setCamInternals(float angle, float ratio, float nearD, float farD);
		void setCamDef(glm::vec3 &p, glm::vec3 &l, glm::vec3 &u);
		Intersection PointInFrustum(const glm::vec3 &p);
		Intersection SphereInFrustum(const glm::vec3 &p, float raio);
		Intersection AABoxInFrustum(const AABB &b);

		void drawPoints();
		void drawLines();
		void drawPlanes();
		void drawNormals();
	};
}

#endif // kgl_view_frustum_h__