#include "mesh.h"

namespace DigitalSculpt
{
    Mesh::Mesh() 
    {
    }
    
    Mesh::~Mesh()
    {
    }

    void Mesh::Init()
    {
        this->InitTopology();
        this->UpdateGeometry();
    }

    void Mesh::InitTopology()
    {

    }

    void Mesh::UpdateGeometry(Uint32Array* iFaces, Uint32Array* iVerts)
    {
        updateFacesAabbAndNormal(*iFaces);
        updateVerticesNormal(*iVerts);
        updateOctree(*iFaces);

        if (_renderData)
        {
            updateDuplicateGeometry(*iVerts);
            updateDrawArrays(iFaces);
        }
    }

    /// <summary>
    /// 根据传递进来的面信息，计算AABB信息和面法线信息
    /// </summary>
    /// <param name="iFaces"></param>
    void Mesh::updateFacesAabbAndNormal(Uint32Array* iFaces)
    {
        // 拿到面法线数组准备填充
      //  Float32Array& faceNormals = getFaceNormals(); mesh_data_->_faceNormalsXYZ;
       // Float32Array& faceBoxes = getFaceBoxes(); //mesh_data_->_faceBoxes
        // Float32Array& faceCenters = getFaceCenters(); // mesh_data_->_faceCentersXYZ
        //const Float32Array& vAr = getVertices(); // mesh_data_->positions_
        //const Uint32Array& fAr = getFaces(); // mesh_data_->_facesABCD

        // 如果传递进来的要更新的面列表为空，那么就表示更新整个mesh的面
        bool full = iFaces->size() == 0;
        std::uint32_t nbFaces = full ? mesh_data_->_nbFaces /*getNbFaces()*/ : iFaces->size();

        // 遍历每一个面
        for (std::uint32_t i = 0; i < nbFaces; ++i)
        {
            // 如果full为true，那么这个循环遍历值就是面索引
            std::uint32_t ind = full ? i : iFaces->at(i);//[i] ;
            std::uint32_t idTri = ind * 3;
            std::uint32_t idFace = ind * 4;
            std::uint32_t idBox = ind * 6;

            std::uint32_t ind1 = mesh_data_->_facesABCD[idFace + 0]/* *3 */;
            std::uint32_t ind2 = mesh_data_->_facesABCD[idFace + 1]/* *3 */;
            std::uint32_t ind3 = mesh_data_->_facesABCD[idFace + 2]/* *3 */;
            std::uint32_t ind4 = mesh_data_->_facesABCD[idFace + 3];

            bool isQuad = ind4 != Utils::TRI_INDEX;

            if (isQuad)
                ind4 *= 3;

            // 拿到三个三角形的顶点
            float v1x = mesh_data_->positions_[ind1/*+ 0*/].x;
            float v1y = mesh_data_->positions_[ind1/*+ 1*/].y;
            float v1z = mesh_data_->positions_[ind1/*+ 2*/].z;

            float v2x = mesh_data_->positions_[ind2/*+ 0*/].x;
            float v2y = mesh_data_->positions_[ind2/*+ 1*/].y;
            float v2z = mesh_data_->positions_[ind2/*+ 2*/].z;

            float v3x = mesh_data_->positions_[ind3/*+ 0*/].x;
            float v3y = mesh_data_->positions_[ind3/*+ 1*/].y;
            float v3z = mesh_data_->positions_[ind3/*+ 2*/].z;

            // 计算法线 。首先第一步计算三角形的三个边的连线向量
            float ax = v2x - v1x;
            float ay = v2y - v1y;
            float az = v2z - v1z;
            float bx = v3x - v1x;
            float by = v3y - v1y;
            float bz = v3z - v1z;

            // 计算叉积
            float crx = ay * bz - az * by;
            float cry = az * bx - ax * bz;
            float crz = ax * by - ay * bx;

            float cr_len = std::sqrt(crz * crz + cry * cry + crx * crx);
            crx /= cr_len;
            cry /= cr_len;
            crz /= cr_len;

            // compute boxes
            float xmax = 0.0f, xmin = 0.0f;
            float ymax = 0.0f, ymin = 0.0f;
            float zmax = 0.0f, zmin = 0.0f;

            Utils::FindMaxMin(v1x, v2x, v3x, xmax, xmin);
            Utils::FindMaxMin(v1y, v2y, v3y, ymax, ymin);
            Utils::FindMaxMin(v1z, v2z, v3z, zmax, zmin);

            if (isQuad)
            {
                float v4x = mesh_data_->positions_[ind4/*+ 0*/].x;
                float v4y = mesh_data_->positions_[ind4/*+ 1*/].y;
                float v4z = mesh_data_->positions_[ind4/*+ 2*/].z;
                if (v4x < xmin) xmin = v4x;
                if (v4x > xmax) xmax = v4x;
                if (v4y < ymin) ymin = v4y;
                if (v4y > ymax) ymax = v4y;
                if (v4z < zmin) zmin = v4z;
                if (v4z > zmax) zmax = v4z;
                ax = v3x - v4x;
                ay = v3y - v4y;
                az = v3z - v4z;
                crx += ay * bz - az * by;
                cry += az * bx - ax * bz;
                crz += ax * by - ay * bx;
            }

            // normals
            mesh_data_->_faceNormalsXYZ[idTri/*+ 0*/].x = crx;
            mesh_data_->_faceNormalsXYZ[idTri/*+ 1*/].y = cry;
            mesh_data_->_faceNormalsXYZ[idTri/*+ 2*/].z = crz;

            // boxes
            mesh_data_->_faceBoxes[idBox + 0] = xmin;
            mesh_data_->_faceBoxes[idBox + 1] = ymin;
            mesh_data_->_faceBoxes[idBox + 2] = zmin;
            mesh_data_->_faceBoxes[idBox + 3] = xmax;
            mesh_data_->_faceBoxes[idBox + 4] = ymax;
            mesh_data_->_faceBoxes[idBox + 5] = zmax;

            // compute centers
            mesh_data_->_faceCentersXYZ[idTri/*+ 0*/].x = (xmin + xmax) * 0.5f;
            mesh_data_->_faceCentersXYZ[idTri/*+ 1*/].y = (ymin + ymax) * 0.5f;
            mesh_data_->_faceCentersXYZ[idTri/*+ 2*/].z = (zmin + zmax) * 0.5f;
        }
    }
}