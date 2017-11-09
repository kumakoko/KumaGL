// 模型类
#ifndef kgl_static_model_h__
#define kgl_static_model_h__

#include "kgl_static_mesh.h"
#include "kgl_vertex_type.h"

namespace kgl
{
    class StaticModel
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="Model"/> class.
        /// </summary>
        /// <param name="vt">The vt.</param>
        /// <param name="path">The path.</param>
        StaticModel(VertexType vt, const char* path);

        /// <summary>
        /// Draws this instance.
        /// </summary>
        void Draw();

        /// <summary>
        /// 把某个shader程序，作用到模型中某个指定的mesh上
        /// </summary>
        /// <param name="index">本模型中，待使用shader的那个mesh的数组索引</param>
        /// <param name="shader">待使用的shader</param>
        void ApplyShaderToMesh(std::size_t index, GPUProgramSPtr shader);

        /// <summary>
        /// 把某个shader程序，作用到模型中每一个mesh上
        /// </summary>
        /// <param name="shader">待使用的shader</param>
        void ApplyShaderToModel(GPUProgramSPtr shader);

        /// <summary>
        /// Gets the mesh shader.
        /// </summary>
        /// <param name="index">The index.</param>
        /// <returns>GPUProgramSPtr.</returns>
        GPUProgramSPtr GetMeshShader(std::size_t index);

        /// <summary>
        /// Gets the mesh count.
        /// </summary>
        /// <returns>const std.size_t.</returns>
        inline const std::size_t GetMeshCount() const
        {
            return meshes_.size();
        }

        /// <summary>
        /// Gets the mesh count.
        /// </summary>
        /// <returns>std.size_t.</returns>
        inline std::size_t GetMeshCount()
        {
            return meshes_.size();
        }

        /// <summary>
        ///  给指定的mesh使用的shader，指定一个纹理对象。
        /// </summary>
        /// <param name="mesh_index">The texture.</param>
        /// <param name="texture">The texture.</param>
        /// <param name="uniform_var_name">The uniform_var_name.</param>
        /// <param name="slot_index">The slot_index.</param>
        void ApplyTextureToMesh(std::size_t mesh_index,TextureSPtr texture, const char* uniform_var_name, GLuint slot_index);

        /// <summary>
        /// 给model下的所有的mesh使用的shader，指定一个纹理对象。
        /// </summary>
        /// <param name="texture">The texture.</param>
        /// <param name="uniform_var_name">The uniform_var_name.</param>
        /// <param name="slot_index">The slot_index.</param>
        void ApplyTextureToModel(TextureSPtr texture, const char* uniform_var_name, GLuint slot_index);

        /// <summary>
        ///  给指定的mesh使用的shader，指定一个矩阵。
        /// </summary>
        /// <param name="mesh_index"></param>
        /// <param name="matrix_data_pointer">存放着矩阵数据的数组首指针</param>
        /// <param name="uniform_var_name">代码中该矩阵变量的名字</param>
        /// <param name="need_transpose">传递进来的矩阵需要转置吗</param>
        void ApplyMatrixToMesh(std::size_t mesh_index, const GLfloat* matrix_data_pointer, const char* uniform_var_name, bool need_transpose = false);

        /// <summary>
        /// 给model下的所有的mesh使用的shader，指定一个矩阵。
        /// </summary>
        /// <param name="matrix_data_pointer">The matrix_data_pointer.</param>
        /// <param name="uniform_var_name">The uniform_var_name.</param>
        /// <param name="need_transpose">The need_transpose.</param>
        void ApplyMatrixToModel(const GLfloat* matrix_data_pointer, const char* uniform_var_name, bool need_transpose = false);

        /// <summary>
        /// 把model中的某个mesh的自身局部变换坐标作用到某个mesh中
        /// </summary>
        /// <param name="mesh_index">The mesh_index.</param>
        /// <param name="uniform_var_name">The uniform_var_name.</param>
        /// <param name="need_transpose">The need_transpose.</param>
        void ApplyLocalTransformMatrixToMesh(std::size_t mesh_index, const char* uniform_var_name, bool need_transpose = false);

        /// <summary>
        /// 把model中的每个mesh的自身局部变换坐标作用到它们上
        /// </summary>
        /// <param name="uniform_var_name">The uniform_var_name.</param>
        /// <param name="need_transpose">The need_transpose.</param>
        void ApplyLocalTransformMatrixToModel(const char* uniform_var_name, bool need_transpose = false);

        /// <summary>
        /// Applies the float.
        /// </summary>
        /// <param name="mesh_index">The texture.</param>
        /// <param name="float_data">The float_data.</param>
        /// <param name="uniform_var_name">The uniform_var_name.</param>
        void ApplyFloatToMesh(std::size_t mesh_index, GLfloat float_data, const char* uniform_var_name);

        void ApplyFloatToModel(GLfloat float_data, const char* uniform_var_name);

        /// <summary>
        /// Applies the vector2.
        /// </summary>
        /// <param name="mesh_index">The texture.</param>
        /// <param name="vector2_data_pointer">The vector2_data_pointer.</param>
        /// <param name="uniform_var_name">The uniform_var_name.</param>
        void ApplyVector2ToMesh(std::size_t mesh_index, const GLfloat* vector2_data_pointer, const char* uniform_var_name);

        void ApplyVector2ToModel(const GLfloat* vector2_data_pointer, const char* uniform_var_name);

        /// <summary>
        /// Applies the vector3.
        /// </summary>
        /// <param name="mesh_index">The texture.</param>
        /// <param name="vector3_data_pointer">The vector3_data_pointer.</param>
        /// <param name="uniform_var_name">The uniform_var_name.</param>
        void ApplyVector3ToMesh(std::size_t mesh_index, const GLfloat* vector3_data_pointer, const char* uniform_var_name);

        void ApplyVector3ToModel(const GLfloat* vector3_data_pointer, const char* uniform_var_name);

        /// <summary>
        /// Applies the vector4.
        /// </summary>
        /// <param name="mesh_index">The texture.</param>
        /// <param name="vector4_data_pointer">The vector4_data_pointer.</param>
        /// <param name="uniform_var_name">The uniform_var_name.</param>
        void ApplyVector4ToMesh(std::size_t mesh_index, const GLfloat* vector4_data_pointer, const char* uniform_var_name);

        void ApplyVector4ToModel(const GLfloat* vector4_data_pointer, const char* uniform_var_name);

        /// <summary>
        /// Applies the int.
        /// </summary>
        /// <param name="mesh_index">The texture.</param>
        /// <param name="int_data">The int_data.</param>
        /// <param name="uniform_var_name">The uniform_var_name.</param>
        void ApplyIntToMesh(std::size_t mesh_index, GLint int_data, const char* uniform_var_name);

        void ApplyIntToModel(GLint int_data, const char* uniform_var_name);

        /// <summary>
        /// 要使用此方法去设置shader中的材质，则shader代码务必使用material.glsl中定义的Material结构
        /// </summary>
        /// <param name="mesh_index">The texture.</param>
        /// <param name="material">The material.</param>
        /// <param name="uniform_var_name">The uniform_var_name.</param>
        void ApplyMaterialToMesh(std::size_t mesh_index, const Material* material, const char* uniform_var_name);

        void ApplyMaterialToModel(const Material* material, const char* uniform_var_name);

        /// <summary>
        /// Applies the directional light.
        /// </summary>
        /// <param name="mesh_index">The texture.</param>
        /// <param name="light">The light.</param>
        /// <param name="uniform_var_name">The uniform_var_name.</param>
        void ApplyDirectionalLightToMesh(std::size_t mesh_index, const DirectionalLight* light, const char* uniform_var_name);

        void ApplyDirectionalLightToModel(const DirectionalLight* light, const char* uniform_var_name);

        /// <summary>
        /// Applies the point light.
        /// </summary>
        /// <param name="light">The light.</param>
        /// <param name="uniform_var_name">The uniform_var_name.</param>
        void ApplyPointLightToMesh(std::size_t mesh_index, const PointLight* light, const char* uniform_var_name);

        void ApplyPointLightToModel(const PointLight* light, const char* uniform_var_name);

        /// <summary>
        /// Applies the spot light.
        /// </summary>
        /// <param name="mesh_index">The texture.</param>
        /// <param name="light">The light.</param>
        /// <param name="uniform_var_name">The uniform_var_name.</param>
        void ApplySpotLightToMesh(std::size_t mesh_index, const SpotLight* light, const char* uniform_var_name);

        void ApplySpotLightToModel(const SpotLight* light, const char* uniform_var_name);

        void UseShaderForMesh(std::size_t mesh_index);

        void UseShader();

        void DrawSubset(std::size_t mesh_index);
    protected:
        /// <summary>
        /// Loads the model.
        /// </summary>
        /// <param name="path">The path.</param>
        void LoadModel(const std::string& path);

        /// <summary>
        /// 处理的模型的某个节点
        /// </summary>
        /// <param name="scene">模型的层级结构首指针</param>
        /// <param name="node">待处理的模型的某个节点</param>
        /// <param name="parent_trransform">待处理的模型的某个节点的父节点的TRS变换矩阵</param>
        void ProcessNode(const aiScene* scene, const aiNode* node, const glm::mat4& parent_trransform);

        /// <summary>
        /// Processes the mesh.
        /// </summary>
        /// <param name="scene">模型的层级结构首指针</param>
        /// <param name="mesh">待处理的模型的某个mesh</param>
        /// <param name="model_transform">待处理的模型的某个节点的父节点的TRS变换矩阵</param>
        /// <returns>kgl.Mesh *.</returns>
        StaticMesh* ProcessMesh(const aiScene* scene, aiMesh* mesh, const glm::mat4& model_transform);

        /// <summary>
        /// Loads the material textures.
        /// </summary>
        /// <param name="mat">The mat.</param>
        /// <param name="type">The type.</param>
        /// <param name="type_name">The type_name.</param>
        /// <param name="params">The parameters.</param>
        /// <returns>std.vector&lt;_Ty, _Alloc&gt;.</returns>
        std::vector<TextureSPtr> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& type_name, const TextureParams& params);

        /// <summary>
        /// Reads the vertex data.
        /// </summary>
        /// <param name="vt">The vt.</param>
        /// <param name="mesh">The mesh.</param>
        /// <param name="vertex_byte_count">The vertex_byte_count.</param>
        /// <returns>uint8_t *.</returns>
        uint8_t* ReadVertexData(VertexType vt, const aiMesh* mesh, uint32_t* vertex_byte_count);

        /// <summary>
        /// Reads the vertex data p.
        /// </summary>
        /// <param name="mesh">The mesh.</param>
        /// <param name="vertex_byte_count">The vertex_byte_count.</param>
        /// <returns>uint8_t *.</returns>
        uint8_t* ReadVertexDataP(const aiMesh* mesh, uint32_t* vertex_byte_count);

        /// <summary>
        /// Reads the vertex data pc.
        /// </summary>
        /// <param name="mesh">The mesh.</param>
        /// <param name="vertex_byte_count">The vertex_byte_count.</param>
        /// <returns>uint8_t *.</returns>
        uint8_t* ReadVertexDataPC(const aiMesh* mesh, uint32_t* vertex_byte_count);

        /// <summary>
        /// Reads the vertex data pn.
        /// </summary>
        /// <param name="mesh">The mesh.</param>
        /// <param name="vertex_byte_count">The vertex_byte_count.</param>
        /// <returns>uint8_t *.</returns>
        uint8_t* ReadVertexDataPN(const aiMesh* mesh, uint32_t* vertex_byte_count);

        /// <summary>
        /// Reads the vertex data PNTB.
        /// </summary>
        /// <param name="mesh">The mesh.</param>
        /// <param name="vertex_byte_count">The vertex_byte_count.</param>
        /// <returns>uint8_t *.</returns>
        uint8_t* ReadVertexDataPNTB(const aiMesh* mesh, uint32_t* vertex_byte_count);

        /// <summary>
        /// Reads the vertex data PNTB t1.
        /// </summary>
        /// <param name="mesh">The mesh.</param>
        /// <param name="vertex_byte_count">The vertex_byte_count.</param>
        /// <returns>uint8_t *.</returns>
        uint8_t* ReadVertexDataPNTBT1(const aiMesh* mesh, uint32_t* vertex_byte_count);

        /// <summary>
        /// Reads the vertex data PNTB t2.
        /// </summary>
        /// <param name="mesh">The mesh.</param>
        /// <param name="vertex_byte_count">The vertex_byte_count.</param>
        /// <returns>uint8_t *.</returns>
        uint8_t* ReadVertexDataPNTBT2(const aiMesh* mesh, uint32_t* vertex_byte_count);

        /// <summary>
        /// Reads the vertex data PNTB t3.
        /// </summary>
        /// <param name="mesh">The mesh.</param>
        /// <param name="vertex_byte_count">The vertex_byte_count.</param>
        /// <returns>uint8_t *.</returns>
        uint8_t* ReadVertexDataPNTBT3(const aiMesh* mesh, uint32_t* vertex_byte_count);

        /// <summary>
        /// Reads the vertex data PNTB t4.
        /// </summary>
        /// <param name="mesh">The mesh.</param>
        /// <param name="vertex_byte_count">The vertex_byte_count.</param>
        /// <returns>uint8_t *.</returns>
        uint8_t* ReadVertexDataPNTBT4(const aiMesh* mesh, uint32_t* vertex_byte_count);

        /// <summary>
        /// Reads the vertex data pn t1.
        /// </summary>
        /// <param name="mesh">The mesh.</param>
        /// <param name="vertex_byte_count">The vertex_byte_count.</param>
        /// <returns>uint8_t *.</returns>
        uint8_t* ReadVertexDataPNT1(const aiMesh* mesh, uint32_t* vertex_byte_count);

        /// <summary>
        /// Reads the vertex data pn t2.
        /// </summary>
        /// <param name="mesh">The mesh.</param>
        /// <param name="vertex_byte_count">The vertex_byte_count.</param>
        /// <returns>uint8_t *.</returns>
        uint8_t* ReadVertexDataPNT2(const aiMesh* mesh, uint32_t* vertex_byte_count);

        /// <summary>
        /// Reads the vertex data pn t3.
        /// </summary>
        /// <param name="mesh">The mesh.</param>
        /// <param name="vertex_byte_count">The vertex_byte_count.</param>
        /// <returns>uint8_t *.</returns>
        uint8_t* ReadVertexDataPNT3(const aiMesh* mesh, uint32_t* vertex_byte_count);

        /// <summary>
        /// Reads the vertex data pn t4.
        /// </summary>
        /// <param name="mesh">The mesh.</param>
        /// <param name="vertex_byte_count">The vertex_byte_count.</param>
        /// <returns>uint8_t *.</returns>
        uint8_t* ReadVertexDataPNT4(const aiMesh* mesh, uint32_t* vertex_byte_count);

        /// <summary>
        /// Checks the has positions.
        /// </summary>
        /// <param name="mesh">The mesh.</param>
        /// <param name="vertex_type">The vertex_type.</param>
        /// <param name="file">The file.</param>
        /// <param name="line">The line.</param>
        void CheckHasPositions(const aiMesh* mesh, const wchar_t* vertex_type, const char* file, uint32_t line);

        /// <summary>
        /// Checks the has normals.
        /// </summary>
        /// <param name="mesh">The mesh.</param>
        /// <param name="vertex_type">The vertex_type.</param>
        /// <param name="file">The file.</param>
        /// <param name="line">The line.</param>
        void CheckHasNormals(const aiMesh* mesh, const wchar_t* vertex_type, const char* file, uint32_t line);

        /// <summary>
        /// Checks the tangent and binormals.
        /// </summary>
        /// <param name="mesh">The mesh.</param>
        /// <param name="vertex_type">The vertex_type.</param>
        /// <param name="file">The file.</param>
        /// <param name="line">The line.</param>
        void CheckTangentAndBinormals(const aiMesh* mesh, const wchar_t* vertex_type, const char* file, uint32_t line);

        /// <summary>
        /// Checks the texture coords.
        /// </summary>
        /// <param name="mesh">The mesh.</param>
        /// <param name="vertex_type">The vertex_type.</param>
        /// <param name="texture_index">The texture_index.</param>
        /// <param name="file">The file.</param>
        /// <param name="line">The line.</param>
        void CheckTextureCoords(const aiMesh* mesh, const wchar_t* vertex_type, uint32_t texture_index, const char* file, uint32_t line);
    protected:
        /// <summary>
        /// 本模型所用到的网格集合 
        /// </summary>
        std::vector<StaticMesh*> meshes_;

        /// <summary>
        /// 本模型的所使用的顶点的类型
        /// </summary>
        VertexType vertex_type_;
    };
}

#endif // kgl_static_model_h__