/*!
 * \file kgl_standard_index_generator.h
 * \date 2017/11/24 17:46
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
#ifndef kgl_standard_index_generator_h__
#define kgl_standard_index_generator_h__

#include "kgl_indices_generator.h"

namespace kgl
{
    namespace low_poly_terrain
    {
        /// <summary>
        /// 给定的地形生成一个标准的顶点索引数据。每一个地形格由两个三角形组成。
        /// 分别是左上三角形和右下三角形
        /// </summary>
        /// <seealso cref="IndicesGenerator" />
        class StandardIndexGenerator : public IndicesGenerator
        {
        public:
            /// <summary>
            /// Generates the index buffer.
            /// </summary>
            /// <param name="vertexCount">The vertex count.</param>
            /// <param name="indices_array">The indices_array.</param>
            virtual void GenerateIndexBuffer(int vertex_count, std::vector<int>& indices_array) override;
        private:
            /// <summary>
            /// 六个顶点索引值分别对应于左上角，左下角，右下角；左上角，右下角，右上角
            /// 以逆时针的形式存储顶点索引，组成三角形，如下所示
            /// 03____5
            /// | \   |
            /// |  \  |
            /// |   \ |
            /// |    \|
            /// 1____24
            /// </summary>
            /// <param name="indices">待填充计算的顶点索引数组.</param>
            /// <param name="pointer">返回下一个矩形的顶点索引起始索引值</param>
            /// <param name="top_left">左上角顶点索引值</param>
            /// <param name="top_right">右上角顶点索引值</param>
            /// <param name="bottom_left">The bottom_left.</param>
            /// <param name="bottom_right">The bottom_right.</param>
            /// <returns>int.</returns>
            int StoreQuad(std::vector<int>& indices, int pointer, int top_left, int top_right, int bottom_left, int bottom_right);
        };

        typedef std::shared_ptr<StandardIndexGenerator> StandardIndexGeneratorSPtr;
    }
}

#endif // kgl_standard_index_generator_h__