/*!
 * \file kgl_indices_generator.h
 * \date 2017/11/24 17:30
 *
 * \author xiongxinke
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef kgl_indices_generator_h__
#define kgl_indices_generator_h__

namespace kgl
{
	namespace low_poly_terrain
	{
		class IndicesGenerator
		{
		public:
			/// <summary>
			/// 根据给定的顶点个数，生成对应的索引坐标值
			/// </summary>
			/// <param name="vertex_count">待生成索引值的顶点个数值</param>
			/// <param name="indices_array">存储顶点索引</param>
			virtual void generateIndexBuffer(int vertex_count, std::vector<int>& indices_array) = 0;
		};

		typedef std::shared_ptr<IndicesGenerator> IndicesGeneratorSPtr;
	}
}

#endif // kgl_indices_generator_h__
