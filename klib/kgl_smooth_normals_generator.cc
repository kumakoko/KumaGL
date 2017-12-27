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