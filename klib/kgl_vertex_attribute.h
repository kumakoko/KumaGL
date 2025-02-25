﻿/**************************************************************************************************************************
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
/*!
* \file kgl_vertex_attribute.h
* \date 2017/12/31 14:02
*
* \author www.xionggf.com
* Contact: sun_of_lover@sina.com
*
* \brief
*
* TODO: 描述了顶点属性的详细信息的数据结构体
*
* \note
*/

#ifndef kgl_vertex_attribute_h__
#define kgl_vertex_attribute_h__

namespace kgl
{
    struct VertexAttribute
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="VertexAttribute"/> struct.
        /// </summary>
        /// <param name="index">The index.</param>
        /// <param name="normalized">The normalized.</param>
        /// <param name="type">The type.</param>
        /// <param name="size">The size.</param>
        /// <param name="stride">The stride.</param>
        /// <param name="pointer">The pointer.</param>
        VertexAttribute(GLuint index, GLboolean normalized, GLenum type, GLint size, GLsizei stride, const GLvoid* pointer);
        
        /// <summary>
        /// Initializes a new instance of the <see cref="VertexAttribute"/> struct.
        /// </summary>
        VertexAttribute();

        /// <summary>
        /// 指定要配置顶点的哪一个属性。例如，在顶点着色器中使用layout(location = 0)定义了
        /// 顶点的“位置（position）属性”的location值为0。因为希望把数据传递到这个顶点属性
        /// 中，所以在这里要跟着shader代码里面的location的值，同样填0。
        /// </summary>
        GLuint index;

        /// <summary>
        /// 指定了每一个顶点的属性有多少个分量组成。例如，顶点的位置属性
        /// 是vec3类型，它由3个float类型的数据组成。所以这里可以填3
        /// </summary>
        GLint size;

        /// <summary>
        /// 本参数指定本待配置的“顶点的属性”所使用的数据的类型，例如
        /// 顶点的位置属性使用32位浮点数的话。这里则指定值为GL_FLOAT。
        /// </summary>
        GLenum type;

        /// <summary>
        /// 本参数指定是否希望数据被标准化。如果设置为GL_TRUE，所有数
        /// 据都会被映射到0（对于有符号型signed数据是 - 1）到1之间
        /// </summary>
        GLboolean normalized;

        /// <summary>
        /// 本参数即是指步长。本参数指定在连续的“顶点属性”描述字节之间，间隔有多少个字节。
        /// 例如以上面的参数所举的例子为例。由于下个位置数据在3个GLfloat后面的位置，所以步长
        /// 的值为“3 * sizeof(GLfloat)”。另外，如果知道这个数组是紧密排列的（在两个顶点属性
        /// 之间没有空隙），也可以设置为0来让OpenGL决定具体步长是多少（注意：只有当数值是紧密
        /// 排列时才可用）。每当有更多的顶点属性，就必须小心地定义每个顶点属性之间的空间。
        /// </summary>
        GLsizei stride;

        /// <summary>
        /// 本参数指定本顶点属性描述的顶点数据，在缓冲中起始位置的偏移量。
        /// </summary>
        const GLvoid* pointer;
    };
}
#endif // kgl_vertex_attribute_h__