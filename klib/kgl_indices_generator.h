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
			virtual void GenerateIndexBuffer(int vertex_count, std::vector<int>& indices_array) = 0;
		};

		typedef std::shared_ptr<IndicesGenerator> IndicesGeneratorSPtr;
	}
}

#endif // kgl_indices_generator_h__
