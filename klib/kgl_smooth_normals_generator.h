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