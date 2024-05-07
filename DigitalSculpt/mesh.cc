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
        updateFacesAabbAndNormal(iFaces);
        updateVerticesNormal(*iVerts);
        updateOctree(*iFaces);

        if (_renderData)
        {
            updateDuplicateGeometry(*iVerts);
            updateDrawArrays(iFaces);
        }
    }


    /** Computes faces ring around vertices */
    void Mesh::initFaceRings()
    {
        //const Uint32Array& fAr = getFaces(); // mesh_data_->_facesABCD;
        std::uint32_t vertices_count = mesh_data_->vertices_count();//getNbVertices();
        //std::uint32_t nbFaces = mesh_data_->faces_count_;//getNbFaces();
        std::uint32_t i = 0;
        std::uint32_t id = 0;
        Uint32Array countRing;
        countRing.resize(vertices_count, 0); //var countRing = new Uint32Array(vertices_count);

        /*    
         E---F--G
          \  | /|     
           \ |/ |
            \A---B    
             |   |
             C---D
        */

        // 遍历本mesh中的每一个面
        for (i = 0; i < mesh_data_->faces_count_; ++i)
        {
            id = i * 4;

            // mesh_data_->_facesABCD[id + 0] 为面的其中一个顶点，例如叫顶点A的索引，这里
            // 表示顶点A有被作为一个面的顶点，所以计数加1，另外三个顶点也是如此原理
            // 如在上面的注释示意图所示，顶点A被 EFA,FGA,GBA,BDCA四个面共点，所以
            // count_ring[A]应为4
            countRing[mesh_data_->_facesABCD[id + 0]]++; 
            countRing[mesh_data_->_facesABCD[id + 1]]++;
            countRing[mesh_data_->_facesABCD[id + 2]]++;

            // 
            std::uint32_t i4 = mesh_data_->_facesABCD[id + 3]; 
            if (i4 != Utils::TRI_INDEX)
                countRing[i4]++;
        }
        //_vrfStartCount
        //Uint32Array& ringFace = getVerticesRingFaceStartCount(); // mesh_data_->_vrfStartCount

        std::uint32_t acc = 0;
        // 遍历每一个顶点，得到该顶点被多少个面所共点
        for (i = 0; i < vertices_count; ++i)
        {
            // _vrfStartCount记录着每一个顶点的被面共点的信息对
            // 两个uint32类型为一对，每对的第2个数值为该顶点被几个面所共点
            // 第1个数值的计算方式为，该顶点前面的顶点的被面共点数之和
            std::uint32_t count = countRing[i];
            mesh_data_->_vrfStartCount[i * 2 + 0] = acc;
            mesh_data_->_vrfStartCount[i * 2 + 1] = count;
            acc += count;
        }

        //var vrf = new Uint32Array(Utils.getMemory(4 * nbFaces * 6), 0, nbFaces * 6);
        Uint32Array vrf;
        vrf.resize(mesh_data_->faces_count_ * 6, 0);
        acc = 0;

        // 遍历每一个面
        for (i = 0; i < mesh_data_->faces_count_; ++i)
        {
            id = i * 4;

            // 拿到每一个面的四个顶点
            std::uint32_t iv1 = mesh_data_->_facesABCD[id + 0];
            std::uint32_t iv2 = mesh_data_->_facesABCD[id + 1];
            std::uint32_t iv3 = mesh_data_->_facesABCD[id + 2];
            std::uint32_t iv4 = mesh_data_->_facesABCD[id + 3];
            vrf[mesh_data_->_vrfStartCount[iv1 * 2] + (--countRing[iv1])] = i;
            vrf[mesh_data_->_vrfStartCount[iv2 * 2] + (--countRing[iv2])] = i;
            vrf[mesh_data_->_vrfStartCount[iv3 * 2] + (--countRing[iv3])] = i;

            if (iv4 != Utils::TRI_INDEX)
            {
                vrf[mesh_data_->_vrfStartCount[iv4 * 2] + (--countRing[iv4])] = i;
                ++acc;
            }
        }

        //_meshData->_vertRingFace = new Uint32Array(vrf.subarray(0, nbFaces * 3 + acc));
        std::uint32_t* tmp = Utils::subarray<std::uint32_t>(vrf, 0, nbFaces * 3 + acc);
        Array1D<std::uint32_t>* array1d = new Array1D<std::uint32_t>(tmp);
        _meshData->_vertRingFace = array1d;
        delete[] tmp;
    }

    void Mesh::updateVerticesNormal(const Uint32Array* iVerts)
    {
        const Uint32Array& vrfStartCount = getVerticesRingFaceStartCount();
        IUint32ArrayND* vertRingFace = getVerticesRingFace();
        bool is2DArray = vertRingFace->Dimension() == 2;

        /*
        * 下面的这句表示，在Java中，vertRingFace可能有两种类型，一种是UInt32Array，一种是原生的Array
        * 在Mesh.js中，vertRingFace是Uint32Array类型，是一个一维数组
        * 在MeshDynmic.js中，vertRingFace是原生Array类型，且是这个Array里面的每一个元素，又是一个数组，即是一个二维数组
        * 用C#转写，分别用了Array1D和Array2D这两个类去代替
        */
        //var ringFaces = vertRingFace instanceof Array ? vertRingFace : null;
        //Float32Array& nAr = getNormals(); mesh_data_->normals_
        //const Float32Array& faceNormals = getFaceNormals(); //  mesh_data_->_faceNormalsXYZ
       

        bool full = iVerts->size() == 0; //var full = iVerts == = undefined;
        std::uint32_t nbVerts = full ? mesh_data_->positions_.size()/* getNbVertices()*/ : iVerts->size();

        if (is2DArray)
        {
            Uint32Array2D* array2D = dynamic_cast<Uint32Array2D*>(vertRingFace);// as Array2D<int>;

            for (std::uint32_t i = 0; i < nbVerts; ++i)
            {
                int ind = full ? i : static_cast<int>(iVerts->at(i));
                std::uint32_t* ring = (*array2D)[ind];//int[] ring = array2D[ind];
                int end = static_cast<int>(Utils::GetArrayElementsCount(ring));//int end = ring.Length;
                int start = 0;

                float nx = 0.0f, ny = 0.0f, nz = 0.0f;

                for (int j = start; j < end; ++j)
                {
                    // 此时的vertRingFace
                    int id = static_cast<int>(ring[j] * 3);
                    nx += mesh_data_->_faceNormalsXYZ[id/*+ 0*/].x;
                    ny += mesh_data_->_faceNormalsXYZ[id/*+ 1*/].y;
                    nz += mesh_data_->_faceNormalsXYZ[id/*+ 2*/].z;
                }

                float len = static_cast<float>(end - start);
                if (len != 0.0f)
                    len = 1.0f / len;

                ind *= 3;

                mesh_data_->normals_[ind/*+ 0*/].x = nx * len;
                mesh_data_->normals_[ind/*+ 1*/].y = ny * len;
                mesh_data_->normals_[ind/*+ 2*/].z = nz * len;
            }                                   
        }
        else
        {
            Uint32Array1D* array1D = dynamic_cast<Uint32Array1D*>(vertRingFace); //Array1D<int> array1D = vertRingFace as Array1D<int>;

            for (int i = 0; i < nbVerts; ++i)
            {
                int ind = full ? i : static_cast<int>(iVerts->at(i));
                int start = vrfStartCount[ind * 2 + 0];
                int end = start + vrfStartCount[ind * 2 + 1];

                float nx = 0.0f, ny = 0.0f, nz = 0.0f;

                for (int j = start; j < end; ++j)
                {
                    int id = static_cast<int>((*array1D)[j]) * 3;
                    nx += mesh_data_->_faceNormalsXYZ[id/*+ 0*/].x;
                    ny += mesh_data_->_faceNormalsXYZ[id/*+ 1*/].y;
                    nz += mesh_data_->_faceNormalsXYZ[id/*+ 2*/].z;
                }

                float len = static_cast<float>(end - start);

                if (len != 0.0f)
                    len = 1.0f / len;

                ind *= 3;

                mesh_data_->normals_[ind/*+ 0*/].x = nx * len;
                mesh_data_->normals_[ind/*+ 1*/].y = ny * len;
                mesh_data_->normals_[ind/*+ 2*/].z = nz * len;
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
        std::uint32_t nbFaces = full ? mesh_data_->faces_count_ /*getNbFaces()*/ : iFaces->size();

        // 遍历每一个面
        for (std::uint32_t i = 0; i < nbFaces; ++i)
        {
            // 如果full为true，那么这个循环遍历值就是面索引
            std::uint32_t ind = full ? i : iFaces->at(i);//[i] ;
            std::uint32_t idTri = ind /** 3*/; // 一个face index就是对应于一个三角形id，所以不用乘以3
            std::uint32_t idFace = ind * 4;
            std::uint32_t idBox = ind * 6;

            //position用vec3存储，不用乘以3
            std::uint32_t ind1 = mesh_data_->_facesABCD[idFace + 0]/* *3 */; 
            std::uint32_t ind2 = mesh_data_->_facesABCD[idFace + 1]/* *3 */;
            std::uint32_t ind3 = mesh_data_->_facesABCD[idFace + 2]/* *3 */;
            std::uint32_t ind4 = mesh_data_->_facesABCD[idFace + 3];

            bool isQuad = ind4 != Utils::TRI_INDEX;

            /*
            if (isQuad)
                ind4 *= 3;
            */

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