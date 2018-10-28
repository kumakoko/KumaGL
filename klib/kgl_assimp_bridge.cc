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
// assimp和glm之间的一些数据结构之间的转换
#include "kgl_lib_pch.h"
#include "kgl_assimp_bridge.h"

namespace kgl
{
/*
glUniformMatrix2(34)fv函数定义的可以加载4x4矩阵数据，还可以加载像2x2,3x3,3x2,2x4,4x2,3x4和4x3这样的矩阵。

第一个参数是uniform变量的位置。在shader编译后使用glGetUniformLocation()获取；

第二个参数指的是我们要更新的矩阵的个数，我们使用参数1来更新一个矩阵，但我们也可以使用这个函数在一次回调中更新多个矩阵

第三个参数通常会使新手误解，第三个参数指的是矩阵是行优先还是列优先的。行优先指的是矩阵是从顶部开始一行一行给出的，而列
优先是从左边一列一列给出的。C/C++中数组默认是行优先的。也就是说当构建一个二维数组时，在内存中是一行一行存储的，顶部的
行在更低的地址区。

OpenGL开发者通常使用一种内部矩阵布局，叫做列优先矩阵(Column-major Ordering)布局。GLM的默认布局
就是列主序，所以并不需要置换矩阵，我们填GL_FALSE。最后一个参数是真正的矩阵数据首指针，但是GLM并不是
把它们的矩阵储存为OpenGL所希望接受的那种形式，例如我们构造一个在xyz方向上分别平移2，3，4个单位的4x4
平移矩阵。如果按照列优先的写法，那么这个矩阵应该是：

| 1 0 0 2 |
| 0 1 0 3 |
| 0 0 1 4 |
| 0 0 0 1 |

但是GLM的matrix4x4的代码实现，并不是使用一个横4纵4的二维数组去存储数据，而是采用了“有4个列优先
4维向量”的“数组”，假如这个数组称为vecs，也就是说，上面的矩阵：

第1列：（1 0 0 0）存储在vecs[0]所对应的那个4维向量中。
第2列：（0 1 0 0）存储在vecs[1]所对应的那个4维向量中。
第3列：（0 0 1 0）存储在vecs[2]所对应的那个4维向量中。
第4列：（2 3 4 1）存储在vecs[3]所对应的那个4维向量中。

4维向量重载了[]，使用[0][1][2][3]依次取得从左到右的4维向量的4个分量，所以，如果我们从
利用数组的索引值，以下的形式：

[0][0],[0][1],[0][2],[0][3]
[1][0],[1][1],[1][2],[1][3]
[2][0],[2][1],[2][2],[2][3]
[3][0],[3][1],[3][2],[3][3]

打印出数据来的话，就会发现，GLM的矩阵，实质上是列优先的逻辑，但是却看起来像行优先的存储方式
这也就是为什么要使用glm::value_ptr函数去处理GLM矩阵以获取数据首指针的缘故
*/
    void AiMatrix4x4ToGlmMat4(const aiMatrix4x4& ai_matrix, glm::mat4& glm_matrix)
    {
        glm_matrix[0][0] = ai_matrix[0][0]; glm_matrix[0][1] = ai_matrix[1][0]; glm_matrix[0][2] = ai_matrix[2][0]; glm_matrix[0][3] = ai_matrix[3][0];
        glm_matrix[1][0] = ai_matrix[0][1]; glm_matrix[1][1] = ai_matrix[1][1]; glm_matrix[1][2] = ai_matrix[2][1]; glm_matrix[1][3] = ai_matrix[3][1];
        glm_matrix[2][0] = ai_matrix[0][2]; glm_matrix[2][1] = ai_matrix[1][2]; glm_matrix[2][2] = ai_matrix[2][2]; glm_matrix[2][3] = ai_matrix[3][2];
        glm_matrix[3][0] = ai_matrix[0][3]; glm_matrix[3][1] = ai_matrix[1][3]; glm_matrix[3][2] = ai_matrix[2][3]; glm_matrix[3][3] = ai_matrix[3][3];
    }

    void AiMatrix3x3ToGlmMat4(const aiMatrix3x3& ai_matrix, glm::mat4& glm_matrix)
    {
        glm_matrix[0][0] = ai_matrix.a1; glm_matrix[0][1] = ai_matrix.b1; glm_matrix[0][2] = ai_matrix.c1; glm_matrix[0][3] = 0.0f;
        glm_matrix[1][0] = ai_matrix.a2; glm_matrix[1][1] = ai_matrix.b2; glm_matrix[1][2] = ai_matrix.c2; glm_matrix[1][3] = 0.0f;
        glm_matrix[2][0] = ai_matrix.a3; glm_matrix[2][1] = ai_matrix.b3; glm_matrix[2][2] = ai_matrix.c3; glm_matrix[2][3] = 0.0f;
        glm_matrix[3][0] = 0.0f        ; glm_matrix[3][1] = 0.0f        ; glm_matrix[3][2] = 0.0f        ; glm_matrix[3][3] = 1.0f;
    }

    void AiVector3DToGlmVec3(const aiVector3D& ai_vector, glm::vec3 glm_vector)
    {
        glm_vector.x = ai_vector.x;
        glm_vector.y = ai_vector.y;
        glm_vector.z = ai_vector.z;
    }

    glm::vec3  AiVector3DToGlmVec3(const aiVector3D& ai_vector)
    {
        return glm::vec3(ai_vector.x, ai_vector.y, ai_vector.z);
    }
}