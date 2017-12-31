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
#include "kgl_lib_pch.h"
#include "kgl_smooth_normals_generator.h"

namespace kgl
{
	namespace low_poly_terrain
	{
		void SmoothNormalsGenerator::generateNormals(const ublas::matrix<float>& heights, ublas::matrix<glm::vec3>& generated_normal_matrix)
		{
			generated_normal_matrix.resize(heights.size1(), heights.size2(), false);
			
			for (size_t z = 0; z < generated_normal_matrix.size1(); z++) 
			{
				for (size_t x = 0; x < generated_normal_matrix.size2(); x++) 
				{
					generated_normal_matrix(z, x) = calculateNormal(x, z, heights);
				}
			}
		}

		glm::vec3 SmoothNormalsGenerator::calculateNormal(int x, int z, const ublas::matrix<float>& heights)
		{
			float heightL = getHeight(x - 1, z, heights);
			float heightR = getHeight(x + 1, z, heights);
			float heightD = getHeight(x, z - 1, heights);
			float heightU = getHeight(x, z + 1, heights);
			glm::vec3 normal(heightL - heightR, 2.f, heightD - heightU);
			return glm::normalize(normal);
		}

		float SmoothNormalsGenerator::getHeight(int x, int z, const ublas::matrix<float>& heights)
		{
			x = x < 0 ? 0 : x;
			z = z < 0 ? 0 : z;
			x = x >= heights.size2() ? heights.size2() - 1 : x;
			z = z >= heights.size1() ? heights.size1() - 1 : z;
			return heights(z, x);// [z][x];
		}
	}
}