#include "kgl_lib_pch.h"
#include "kgl_standard_index_generator.h"

namespace kgl
{
	namespace low_poly_terrain
	{
		void StandardIndexGenerator::generateIndexBuffer(int vertex_count, std::vector<int>& indices_array)
		{
			int index_count = (vertex_count - 1) * (vertex_count - 1) * 6;
			indices_array.resize(index_count);

			int pointer = 0;
			//  以计算逐行逐列的方式计算出每个三角形的各个顶点的索引值
			for (int col = 0; col < vertex_count - 1; col++)
			{
				for (int row = 0; row < vertex_count - 1; row++)
				{
					int top_left = (row * vertex_count) + col;
					int top_right = top_left + 1;
					int bottom_left = ((row + 1) * vertex_count) + col;
					int bottom_right = bottom_left + 1;
					pointer = storeQuad(indices_array, pointer, top_left, top_right, bottom_left, bottom_right);
				}
			}
		}

		int StandardIndexGenerator::storeQuad(std::vector<int>& indices, int pointer, int top_left, int top_right, int bottom_left, int bottom_right)
		{
			indices[pointer++] = top_left;
			indices[pointer++] = bottom_left;
			indices[pointer++] = bottom_right;
			indices[pointer++] = top_left;
			indices[pointer++] = bottom_right;
			indices[pointer++] = top_right;
			return pointer;
		}
	}
}