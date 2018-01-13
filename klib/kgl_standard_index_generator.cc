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
#include "kgl_standard_index_generator.h"

namespace kgl
{
	namespace low_poly_terrain
	{
		void StandardIndexGenerator::GenerateIndexBuffer(int vertex_count, std::vector<int>& indices_array)
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
					pointer = StoreQuad(indices_array, pointer, top_left, top_right, bottom_left, bottom_right);
				}
			}
		}

		int StandardIndexGenerator::StoreQuad(std::vector<int>& indices, int pointer, int top_left, int top_right, int bottom_left, int bottom_right)
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