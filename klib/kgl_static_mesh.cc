// Mesh类
#include "kgl_lib_pch.h"
#include "kgl_defines.h"
#include "kgl_error.h"
#include "kgl_string_convertor.h"
#include "kgl_static_mesh.h"
#include "kgl_debug_tool.h"

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

    StaticMesh::StaticMesh() :vertex_data_(nullptr), vertex_byte_count_(0), index_data_(nullptr), index_byte_count_(0), vao_(0),vbo_(0),ebo_(0)
    {
    }

    StaticMesh::~StaticMesh()
    {
        this->Clear();
    }

    void StaticMesh::Clear()
    {
        size_t sz = texture_array_.size();

        for (size_t t = 0; t < sz; ++t)
        {
            texture_array_[t].reset();
        }

        texture_array_.clear();

        KGL_SAFE_DELETE_ARRAY(vertex_data_);
        vertex_byte_count_ = 0;

        KGL_SAFE_DELETE_ARRAY(index_data_);
        index_byte_count_ = 0;

        vertex_byte_count_ = 0;
        index_byte_count_ = 0;

        vao_ = 0;
        vbo_ = 0;
        ebo_ = 0;

        shader_.reset();
    }

    void StaticMesh::Setup(VertexType vertex_type, const unsigned char* vertex_data, uint32_t vertex_byte_count, const uint32_t* index_data, uint32_t index_byte_count, const std::vector<TextureSPtr>& texture_array, const glm::mat4& local_transform_matrix)
{
        this->Clear();

        vertex_data_ = new uint8_t[vertex_byte_count];
        memcpy(vertex_data_, vertex_data, vertex_byte_count);

        index_data_ = new uint32_t[index_byte_count / 4];
        memcpy(index_data_, index_data, index_byte_count);

        size_t sz = texture_array.size();

        for (size_t t = 0; t < sz; ++t)
        {
            texture_array_.push_back(texture_array[t]);
        }

        vertex_byte_count_ = vertex_byte_count;
        index_byte_count_ = index_byte_count;
        vertex_type_ = vertex_type;
        // ------------------------------------------------------------------------

        // 创建对应的buffer
        glGenVertexArrays(1, &this->vao_);
        glGenBuffers(1, &this->vbo_);
        glGenBuffers(1, &this->ebo_);

        // 装载数据到顶点缓冲区
        glBindVertexArray(this->vao_);
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo_);
        glBufferData(GL_ARRAY_BUFFER, vertex_byte_count_, vertex_data_, GL_STATIC_DRAW);

        // 装载数据到索引缓冲区
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_byte_count_, index_data_, GL_STATIC_DRAW);

        this->SetupVertexAttributeBuffer(vertex_type_);

        glBindVertexArray(0); // 解除vertex attribute array的绑定

        // 记录下本mesh用到的纹理贴图
        texture_array_ = texture_array;

		local_transform_matrix_ = local_transform_matrix;
#if defined(DEBUG) || defined(_DEBUG)
		DebugTool::PrintGLMMatrix("local transform matrix", local_transform_matrix_);
#endif
    }

    void StaticMesh::Draw()
    {
        shader_->Use();
        glBindVertexArray(vao_);
        glDrawElements(GL_TRIANGLES, index_byte_count_ / sizeof(uint32_t), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void StaticMesh::SetupVertexAttributeBuffer(VertexType vt)
    {
        switch (vt)
        {
        case VERTEX_TYPE_P:this->SetupVertexAttributeBufferP(); break;
        case VERTEX_TYPE_PC:this->SetupVertexAttributeBufferPC(); break;
        case VERTEX_TYPE_PN:this->SetupVertexAttributeBufferPN(); break;
        case VERTEX_TYPE_PNTB:this->SetupVertexAttributeBufferPNTB(); break;
        case VERTEX_TYPE_PNTBT1:this->SetupVertexAttributeBufferPNTBT1(); break;
        case VERTEX_TYPE_PNTBT2:this->SetupVertexAttributeBufferPNTBT2(); break;
        case VERTEX_TYPE_PNTBT3:this->SetupVertexAttributeBufferPNTBT3(); break;
        case VERTEX_TYPE_PNTBT4:this->SetupVertexAttributeBufferPNTBT4(); break;
        case VERTEX_TYPE_PNT1:this->SetupVertexAttributeBufferPNT1(); break;
        case VERTEX_TYPE_PNT2:this->SetupVertexAttributeBufferPNT2(); break;
        case VERTEX_TYPE_PNT3:this->SetupVertexAttributeBufferPNT3(); break;
        case VERTEX_TYPE_PNT4:this->SetupVertexAttributeBufferPNT4(); break;
        }
    }

    void StaticMesh::SetupVertexAttributeBufferP()
	{
		size_t offset_sz = 0;
		glEnableVertexAttribArray(0); // position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexP), (GLvoid*)(offset_sz));
	}
    
	void StaticMesh::SetupVertexAttributeBufferPNTBT1(){}
    void StaticMesh::SetupVertexAttributeBufferPNTBT2(){}
    void StaticMesh::SetupVertexAttributeBufferPNTBT3(){}
    void StaticMesh::SetupVertexAttributeBufferPNTBT4(){}
    void StaticMesh::SetupVertexAttributeBufferPC(){}
    
    void StaticMesh::SetupVertexAttributeBufferPN()
    {
        size_t offset_sz = 0;
        glEnableVertexAttribArray(0); // position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (GLvoid*)(offset_sz));

        offset_sz = sizeof(glm::vec3);
        glEnableVertexAttribArray(1); // normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (GLvoid*)(offset_sz));
    }
    
    void StaticMesh::SetupVertexAttributeBufferPNTB(){}

    void StaticMesh::SetupVertexAttributeBufferPNT1()
    {
        size_t offset_sz = 0;
        glEnableVertexAttribArray(0); // position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNT1), (GLvoid*)(offset_sz));

        offset_sz = sizeof(glm::vec3);
        glEnableVertexAttribArray(1); // normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNT1), (GLvoid*)(offset_sz));

        offset_sz = sizeof(glm::vec3) + sizeof(glm::vec3);
        glEnableVertexAttribArray(2); // texture coord 1
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPNT1), (GLvoid*)(offset_sz));
    }
    
    void StaticMesh::SetupVertexAttributeBufferPNT2()
    {
        size_t offset_sz = 0;
        glEnableVertexAttribArray(0); // position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNT2), (GLvoid*)(offset_sz));

        offset_sz = sizeof(glm::vec3);
        glEnableVertexAttribArray(1); // normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNT2), (GLvoid*)(offset_sz));

        offset_sz = sizeof(glm::vec3) + sizeof(glm::vec3);
        glEnableVertexAttribArray(2); // texture coord 1
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPNT2), (GLvoid*)(offset_sz));

        offset_sz = sizeof(glm::vec3) + sizeof(glm::vec3) + sizeof(glm::vec2) ;
        glEnableVertexAttribArray(3); // texture coord 2
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPNT2), (GLvoid*)(offset_sz));
    }

    void StaticMesh::SetupVertexAttributeBufferPNT3(){}
    void StaticMesh::SetupVertexAttributeBufferPNT4(){}
}