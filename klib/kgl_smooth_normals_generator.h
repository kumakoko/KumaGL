/*!
 * \file kgl_smooth_normals_generator.h
 * \date 2017/11/22 19:04
 *
 * \author xiongxinke
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note Generates smooth normals for all the vertices in a terrain.
*/
/**************************************************************************************************************************
Copyright(C) 2014-2017 www.xionggf.com

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
#ifndef kgl_smooth_normals_generator_h__
#define kgl_smooth_normals_generator_h__

#include "kgl_defines.h"


namespace kgl
{
	namespace low_poly_terrain
	{
		class SmoothNormalsGenerator {

			/**
			* Generates smooth normals for every vertex in the terrain, based on the
			* terrain heights. The normal at each vertex is basically the average of
			* the normals of all the surrounding faces.
			*
			* @param heights - The heights of all the vertices.
			* @return The normals of all the vertices.
			*/
		public: 
			static void generateNormals(const ublas::matrix<float>& heights, ublas::matrix<glm::vec3>& generated_normal_matrix);

		private :
			static glm::vec3 calculateNormal(int x, int z, const ublas::matrix<float>& heights);

			static float getHeight(int x, int z, const ublas::matrix<float>& heights);

		};
	}
}

#endif // kgl_smooth_normals_generator_h__