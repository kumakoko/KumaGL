#include <cmath>
#include <limits>
#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4
#include "glm/mat4x4.hpp" // glm::mat4
#include "glm/ext/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale
#include "glm/ext/matrix_clip_space.hpp" // glm::perspective
#include "glm/ext/scalar_constants.hpp" // glm::pi
#include "boost/algorithm/string.hpp"
#include "math3d/picking.h"
#include "misc/utils.h"
#include "misc/array_1d.h"
#include "misc/array_2d.h"

namespace DigitalSculpt
{
    glm::vec3 _TMP_NEAR = glm::vec3(0.0f);// [0.0, 0.0, 0.0] ;
    glm::vec3 _TMP_NEAR_1 = glm::vec3(0.0f); [0.0, 0.0, 0.0] ;
    glm::vec3 _TMP_FAR = glm::vec3(0.0f); //[0.0, 0.0, 0.0];
    glm::mat4 _TMP_INV = glm::mat4(1.0f);//mat4.create();
    glm::vec3 _TMP_INTER = glm::vec3(0.0f); //[0.0, 0.0, 0.0];
    glm::vec3 _TMP_INTER_1 = glm::vec3(0.0f); //[0.0, 0.0, 0.0];
    glm::vec3 _TMP_V1 = glm::vec3(0.0f); //[0.0, 0.0, 0.0];
    glm::vec3 _TMP_V2 = glm::vec3(0.0f); //[0.0, 0.0, 0.0];
    glm::vec3 _TMP_V3 = glm::vec3(0.0f); //[0.0, 0.0, 0.0];

    std::map<std::string, AlphaInfo*> Picking::ALPHAS;

    AlphaInfo* Picking::addAlpha(kgl::TextureSPtr u8, float width, float height, const std::string& name)
    {
        AlphaInfo* newAlpha = new AlphaInfo();
        newAlpha->_name = name;
        newAlpha->_texture = u8;
        newAlpha->_ratioX = std::max(1.0f, width / height);
        newAlpha->_ratioY = std::max(1.0f, height / width);
        newAlpha->_ratioMax = std::max(newAlpha->_ratioX, newAlpha->_ratioY);
        newAlpha->_width = width;
        newAlpha->_height = height;

        std::uint32_t i = 1;
        while (Picking::ALPHAS[newAlpha->_name])
            newAlpha->_name = name + std::to_string(i++);

        Picking::ALPHAS[newAlpha->_name] = newAlpha;
        // Picking::ALPHAS_NAMES[newAlpha->_name] = newAlpha->_name;
        return newAlpha;
    }

    Picking::Picking(Scene* main, bool xSym)
    {
        _mesh = null; // mesh
        _main = main; // the camera
        _pickedFace = -1; // face picked
        _pickedVertices.clear(); // vertices selected
        _interPoint = glm::vec3(0.0f);// [0.0, 0.0, 0.0] ; // intersection point (mesh local space)
        _rLocal2 = 0.0f; // radius of the selection area (local/object space)
        _rWorld2 = 0.0f; // radius of the selection area (world space)
        _eyeDir = glm::vec3(0.0f);//[0.0, 0.0, 0.0]; // eye direction

        _xSym = xSym;

        _pickedNormal = glm::vec3(0.0f);//[0.0, 0.0, 0.0];
        // alpha stuffs
        _alphaOrigin = glm::vec3(0.0f);//[0.0, 0.0, 0.0];
        _alphaSide = 0.0f;
        _alphaLookAt = glm::mat4();//mat4.create();
        _alpha = nullptr;
    }


    float Picking::getAlpha(float x, float  y, float  z)
    {
        //var alpha = _alpha;
        if (_alpha == nullptr || _alpha->_texture.get() == nullptr) //if (!alpha || !alpha._texture)
            return 1.0f;

        float* m = glm::value_ptr(_alphaLookAt);//var m = _alphaLookAt;
        //var rs = _alphaSide;

        float xn = _alpha->_ratioY * (m[0] * x + m[4] * y + m[8] * z + m[12]) / (_xSym ? -_alphaSide : _alphaSide);
        if (std::fabs(xn) > 1.0f)
            return 0.0f;

        float yn = _alpha->_ratioX * (m[1] * x + m[5] * y + m[9] * z + m[13]) / _alphaSide;
        if (std::fabs(yn) > 1.0f)
            return 0.0f;

        //var aw = alpha._width;
        xn = (0.5 - xn * 0.5) * _alpha->_width;
        yn = (0.5 - yn * 0.5) * _alpha->_height;
        //return alpha._texture[(xn | 0) + _width * (yn | 0)] / 255.0;

    }

    void Picking::updateAlpha(bool keepOrigin)
    {
        glm::vec3& dir = _TMP_V1;
        glm::vec3& nor = _TMP_V2;

        float radius = std::sqrt(_rLocal2);
        _alphaSide = radius * Utils::SQRT1_2;//Math.SQRT1_2;

        dir = _interPoint - _alphaOrigin;// vec3.sub(dir, _interPoint, _alphaOrigin);

        if (glm::length(dir) == 0.0f)return;//if (vec3.len(dir) == = 0) return;
        dir = normalize(dir);//vec3.normalize(dir, dir);

        //var normal = _pickedNormal;
        Utils::scaleAndAdd(dir, dir, _pickedNormal, -glm::dot(dir, _pickedNormal));//vec3.scaleAndAdd(dir, dir, normal, -vec3.dot(dir, normal));
        dir = glm::normalize(dir);//vec3.normalize(dir, dir);

        if (!keepOrigin)
            _alphaOrigin = _interPoint;//vec3.copy(_alphaOrigin, _interPoint);

        Utils::scaleAndAdd(nor, _alphaOrigin, _pickedNormal, radius); //vec3.scaleAndAdd(nor, _alphaOrigin, normal, radius);
        _alphaLookAt = glm::lookAt(_alphaOrigin, nor, dir);//mat4.lookAt(_alphaLookAt, _alphaOrigin, nor, dir);
    }

    bool Picking::intersectionMouseMeshes(std::vector<Mesh*>& meshes/* = _main.getMeshes()*/, float mouseX /*= _main._mouseX*/, float mouseY /*= _main._mouseY*/)
    {
        glm::vec3 vNear = unproject(mouseX, mouseY, 0.0f);
        glm::vec3 vFar = unproject(mouseX, mouseY, 0.1f);
        float nearDistance = std::numeric_limits<float>::max();//Infinity;
        Mesh* nearMesh = nullptr;
        int nearFace = -1;
        std::uint32_t nbMeshes = meshes.size();

        for (std::uint32_t i = 0; i < nbMeshes; ++i)
        {
            Mesh* mesh = meshes[i];
            if (!mesh->isVisible())
                continue;

            _TMP_INV = glm::inverse(mesh->getMatrix());//mat4.invert(_TMP_INV, mesh->getMatrix());
            _TMP_NEAR_1 = _TMP_INV * glm::vec4(vNear, 1.0f); //vec3.transformMat4(_TMP_NEAR_1, vNear, _TMP_INV);
            _TMP_FAR = _TMP_INV * glm::vec4(vFar, 1.0f); //vec3.transformMat4(_TMP_FAR, vFar, _TMP_INV);
            if (!intersectionRayMesh(mesh, _TMP_NEAR_1, _TMP_FAR))
                continue;

            glm::vec3 interTest = getIntersectionPoint();
            float testDistance = glm::distance(_TMP_NEAR_1, interTest) * mesh->getScale();//vec3.dist(_TMP_NEAR_1, interTest) * mesh->getScale();

            if (testDistance < nearDistance)
            {
                nearDistance = testDistance;
                nearMesh = mesh;
                _TMP_INTER_1 = interTest;//vec3.copy(_TMP_INTER_1, interTest);
                nearFace = getPickedFace();
            }
        }

        _mesh = nearMesh;
        _interPoint = _TMP_INTER_1;//vec3.copy(_interPoint, _TMP_INTER_1);
        _pickedFace = nearFace;
        if (nearFace != -1)
            updateLocalAndWorldRadius2();

        // 当nearMesh为null时，!nearMesh为true，!!nearMesh为false
           // 也就是说当nearMesh == null时返回false，所以js语句
           // return !!nearMesh;
           // 可以改写为：
        return nearMesh != null;
    }

    /** Intersection between a ray the mouse position */
    bool Picking::intersectionMouseMesh(Mesh* mesh, float mouseX, float mouseY)
    {
        glm::vec3 vNear = unproject(mouseX, mouseY, 0.0f);
        glm::vec3 vFar = unproject(mouseX, mouseY, 0.1f);
        glm::mat4 matInverse = glm::mat4(1.0f);
        matInverse = glm::inverse(mesh->getMatrix());//mat4.invert(matInverse, mesh->getMatrix());
        vNear = matInverse * glm::vec4(vNear, 1.0f);//vec3.transformMat4(vNear, vNear, matInverse);
        vFar = matInverse * glm::vec4(vFar, 1.0f);//vec3.transformMat4(vFar, vFar, matInverse);
        return intersectionRayMesh(mesh, vNear, vFar);
    }

    bool Picking::intersectionRayMesh(Mesh* mesh, const glm::vec3& vNearOrig, const glm::vec3& vFarOrig)
    {
        // resest picking
        _mesh = nullptr;
        _pickedFace = -1;
        // resest picking
        _TMP_NEAR = vNearOrig;//vec3.copy(_TMP_NEAR, vNearOrig);
        _TMP_FAR = vFarOrig;//vec3.copy(_TMP_FAR, vFarOrig);
        // apply symmetry

        if (_xSym)
        {
            glm::vec3 ptPlane = mesh->getSymmetryOrigin();
            glm::vec3 nPlane = mesh->getSymmetryNormal();
            Geometry::mirrorPoint(_TMP_NEAR, ptPlane, nPlane);
            Geometry::mirrorPoint(_TMP_FAR, ptPlane, nPlane);
        }

        Float32Array& vAr = mesh->getVertices();
        Uint32Array& fAr = mesh->getFaces();
        // compute eye direction
       // glm::vec3& eyeDir = getEyeDirection();
        _eyeDir = _TMP_FAR - _TMP_NEAR;//vec3.sub(eyeDir, _TMP_FAR, _TMP_NEAR);
        _eyeDir = glm::normalize(_eyeDir);//vec3.normalize(eyeDir, eyeDir);
        Uint32Array iFacesCandidates = mesh->intersectRay(_TMP_NEAR, _eyeDir);
        float distance = std::numeric_limits<float>::max();//Infinity;
        std::uint32_t nbFacesCandidates = iFacesCandidates.size();

        for (std::uint32_t i = 0; i < nbFacesCandidates; ++i)
        {
            std::uint32_t indFace = iFacesCandidates[i] * 4;
            std::uint32_t ind1 = fAr[indFace + 0] * 3;
            std::uint32_t ind2 = fAr[indFace + 1] * 3;
            std::uint32_t ind3 = fAr[indFace + 2] * 3;
            _TMP_V1[0] = vAr[ind1 + 0];
            _TMP_V1[1] = vAr[ind1 + 1];
            _TMP_V1[2] = vAr[ind1 + 2];
            _TMP_V2[0] = vAr[ind2 + 0];
            _TMP_V2[1] = vAr[ind2 + 1];
            _TMP_V2[2] = vAr[ind2 + 2];
            _TMP_V3[0] = vAr[ind3 + 0];
            _TMP_V3[1] = vAr[ind3 + 1];
            _TMP_V3[2] = vAr[ind3 + 2];
            float hitDist = Geometry::intersectionRayTriangle(_TMP_NEAR, eyeDir, _TMP_V1, _TMP_V2, _TMP_V3, _TMP_INTER);

            if (hitDist < 0.0f)
            {
                ind2 = fAr[indFace + 3];

                if (ind2 != Utils::TRI_INDEX)
                {
                    ind2 *= 3;
                    _TMP_V2[0] = vAr[ind2];
                    _TMP_V2[1] = vAr[ind2 + 1];
                    _TMP_V2[2] = vAr[ind2 + 2];
                    hitDist = Geometry::intersectionRayTriangle(_TMP_NEAR, eyeDir, _TMP_V1, _TMP_V3, _TMP_V2, _TMP_INTER);
                }
            }

            if (hitDist >= 0.0f && hitDist < distance)
            {
                distance = hitDist;
                _interPoint = _TMP_INTER;// vec3.copy(_interPoint, _TMP_INTER);
                _pickedFace = iFacesCandidates[i];
            }
        }

        if (_pickedFace != -1)
        {
            _mesh = mesh;
            updateLocalAndWorldRadius2();
            return true;
        }_Ctraits<long double>
            _rLocal2 = 0.0f;
        return false;
    }

    Uint32Array& Picking::pickVerticesInSphere(float rLocal2)
    {
        //Mesh* mesh = _mesh;
        Float32Array& vAr = _mesh->getVertices();
        Int32Array& vertSculptFlags = _mesh->getVerticesSculptFlags();
        //_interPoint;//glm::vec3& inter = getIntersectionPoint();

        Uint32Array iFacesInCells = _mesh->intersectSphere(_interPoint, rLocal2, true);
        Uint32Array iVerts = _mesh->getVerticesFromFaces(iFacesInCells);
        std::uint32_t nbVerts = iVerts.size();

        int sculptFlag = ++Utils::SCULPT_FLAG;
        Uint32Array pickedVertices(nbVerts, 0);// = new Uint32Array(Utils.getMemory(4 * nbVerts), 0, nbVerts);
        std::uint32_t acc = 0;
        /* var itx = _interPoint.x;
         var ity = _interPoint.y;
         var itz = _interPoint.z;*/

        for (std::uint32_t i = 0; i < nbVerts; ++i)
        {
            std::uint32_t ind = iVerts[i];
            std::uint32_t j = ind * 3;
            float dx = _interPoint.x - vAr[j + 0];
            float dy = _interPoint.y - vAr[j + 1];
            float dz = _interPoint.z - vAr[j + 2];

            if ((dx * dx + dy * dy + dz * dz) < rLocal2)
            {
                vertSculptFlags[ind] = sculptFlag;
                pickedVertices[acc++] = ind;
            }
        }

        _pickedVertices = Utils::subarray(pickedVertices, 0, acc);//_pickedVertices = new Uint32Array(pickedVertices.subarray(0, acc));
        return _pickedVertices;
    }

    bool Picking::_isInsideSphere(std::uint32_t id, const glm::vec3& inter, float rLocal2) const
    {
        if (id == Utils::TRI_INDEX)
            return false;

        std::uint32_t iv = id * 3;

        const Float32Array& vertices = _mesh->getVertices();
        glm::vec3 v(vertices[iv], vertices[iv + 1], vertices[iv + 2]);
        return glm::distance2(inter, v) <= rLocal2;//vec3.sqrDist(inter, _mesh->getVertices().subarray(iv, iv + 3)) <= rLocal2;
    }

    Uint32Array& Picking::pickVerticesInSphereTopological(float rLocal2)
    {
        //Mesh mesh = _mesh;
        std::uint32_t  nbVertices = _mesh->getNbVertices();
        const Float32Array& vAr = _mesh->getVertices();
        const Uint32Array& fAr = _mesh->getFaces();

        const Uint32Array& vrvStartCount = _mesh->getVerticesRingVertStartCount();
        const IArrayND<std::uint32_t>* vertRingVert = _mesh->getVerticesRingVert();
        bool is2DArray = vertRingVert->Dimension() == 2; //var ringVerts = vertRingVert instanceof Array ? vertRingVert: null;

        Int32Array& vertSculptFlags = _mesh->getVerticesSculptFlags();
        Int32Array& vertTagFlags = _mesh->getVerticesTagFlags();

        int sculptFlag = ++Utils::SCULPT_FLAG;
        int tagFlag = ++Utils::TAG_FLAG;

        //_interPoint//vec3 inter = getIntersectionPoint();
        /*float itx = _interPoint.x;
        float ity = _interPoint.y;
        float itz = _interPoint.z;*/

        Uint32Array pickedVertices(nbVertices, 0);//int[] pickedVertices = new int[nbVertices];//new Uint32Array(Utils.getMemory(4 * nbVertices), 0, nbVertices);
        std::int32_t  idf = getPickedFace() * 4;
        std::int32_t  acc = 1;

        if (_isInsideSphere(fAr[idf], _interPoint, rLocal2))
            pickedVertices[0] = fAr[idf];
        else if (_isInsideSphere(fAr[idf + 1], _interPoint, rLocal2))
            pickedVertices[0] = fAr[idf + 1];
        else if (_isInsideSphere(fAr[idf + 2], _interPoint, rLocal2))
            pickedVertices[0] = fAr[idf + 2];
        else if (_isInsideSphere(fAr[idf + 3], _interPoint, rLocal2))
            pickedVertices[0] = fAr[idf + 3];
        else
            acc = 0;

        if (acc == 1)
        {
            vertSculptFlags[pickedVertices[0]] = sculptFlag;
            vertTagFlags[pickedVertices[0]] = tagFlag;
        }

        const Uint32Array1D* array1d = nullptr;//Array1D<int> array1d = null;
        const Uint32Array2D* array2d = nullptr;

        if (is2DArray)
            array2d = dynamic_cast<const Uint32Array2D*>(vertRingVert);//vertRingVert as Array2D<int>;
        else
            array1d = dynamic_cast<const Uint32Array1D*>(vertRingVert);//vertRingVert as Array1D<int>;

        const uint32_t* ring = nullptr;//int[] ring = null;

        for (std::int32_t i = 0; i < acc; ++i)
        {
            int id = pickedVertices[i];
            int start, end;

            if (is2DArray)
            {
                ring = (*array2d)[id];
                end = Utils::GetArrayElementsCount(ring);//ring.Length;
                start = 0;
            }
            else
            {
                ring = array1d->Array();
                start = vrvStartCount[id * 2];
                end = start + vrvStartCount[id * 2 + 1];
            }

            for (int j = start; j < end; ++j)
            {
                int idv = ring[j];//vertRingVert[j];

                if (vertTagFlags[idv] == tagFlag)
                    continue;

                vertTagFlags[idv] = tagFlag;

                int id3 = idv * 3;
                float dx = _interPoint.x - vAr[id3 + 0];
                float dy = _interPoint.y - vAr[id3 + 1];
                float dz = _interPoint.z - vAr[id3 + 2];

                if ((dx * dx + dy * dy + dz * dz) > rLocal2)
                    continue;

                vertSculptFlags[idv] = sculptFlag;
                pickedVertices[acc++] = idv;
            }
        }

        _pickedVertices = Utils::subarray(pickedVertices, 0, acc);//pickedVertices.subarray(0, acc); //new Uint32Array(pickedVertices.subarray(0, acc));
        return _pickedVertices;
    }

    float Picking::computeWorldRadius2(bool ignorePressure)
    {
        _TMP_INTER = _mesh->getMatrix() * glm::vec4(_interPoint, 1.0f);// vec3.transformMat4(_TMP_INTER, getIntersectionPoint(), _mesh->getMatrix());

        float offsetX = _main->getSculptManager().getCurrentTool().getScreenRadius();

        if (!ignorePressure)
            offsetX *= Tablet.getPressureRadius();

        glm::vec3 screenInter = project(_TMP_INTER);
        return glm::distance2(_TMP_INTER, unproject(screenInter.x + offsetX, screenInter.y, screenInter.z));//vec3.sqrDist(_TMP_INTER, unproject(screenInter[0] + offsetX, screenInter[1], screenInter[2]));
    }

    float Picking::updateLocalAndWorldRadius2()
    {
        if (_mesh == nullptr)
            return;

        _rWorld2 = computeWorldRadius2();
        _rLocal2 = _rWorld2 / _mesh->getScale2();
    }

    glm::vec3 Picking::unproject(float x, float y, float z)
    {
        return _main->getCamera().unproject(x, y, z);
    }

    void Picking::project(glm::vec3 vec)
    {
        return _main->getCamera().project(vec);
    }

    glm::vec3* Picking::computePickedNormal()
    {
        if (_mesh == nullptr || _pickedFace < 0)
            return;

        polyLerp(_mesh->getNormals(), _pickedNormal);
        _pickedNormal = glm::normalize(_pickedNormal);//vec3.normalize(_pickedNormal, _pickedNormal);
        return &_pickedNormal;
    }

    glm::vec3& Picking::polyLerp(const Float32Array& vField, glm::vec3& out)
    {
        const Float32Array& vAr = _mesh->getVertices();
        const Uint32Array& fAr = _mesh->getFaces();
        std::int32_t id = _pickedFace * 4;
        std::uint32_t iv1 = fAr[id + 0] * 3;
        std::uint32_t iv2 = fAr[id + 1] * 3;
        std::uint32_t iv3 = fAr[id + 2] * 3;

        std::uint32_t iv4 = fAr[id + 3];
        bool isQuad = iv4 != Utils::TRI_INDEX;
        if (isQuad)
            iv4 *= 3;

        float len1 = 1.0f / glm::distance(_interPoint, glm::vec3(vAr[iv1], vAr[iv1 + 1], vAr[iv1 + 2]));//vec3.dist(_interPoint, vAr.subarray(iv1, iv1 + 3));
        float len2 = 1.0f / glm::distance(_interPoint, glm::vec3(vAr[iv2], vAr[iv2 + 1], vAr[iv2 + 2]));//vec3.dist(_interPoint, vAr.subarray(iv2, iv2 + 3));
        float len3 = 1.0f / glm::distance(_interPoint, glm::vec3(vAr[iv3], vAr[iv3 + 1], vAr[iv3 + 2]));//vec3.dist(_interPoint, vAr.subarray(iv3, iv3 + 3));
        float len4 = isQuad ? 1.0f / glm::distance(_interPoint, glm::vec3(vAr[iv4], vAr[iv4 + 1], vAr[iv4 + 2])) : 0.0f;//vec3.dist(_interPoint, vAr.subarray(iv4, iv4 + 3)) : 0.0;

        float invSum = 1.0f / (len1 + len2 + len3 + len4);
        out.x = out.y = out.z = 0.0f; //vec3.set(out, 0.0, 0.0, 0.0);
        Utils::scaleAndAdd(out, out, glm::vec3(vAr[iv1], vAr[iv1 + 1], vAr[iv1 + 2])/*vField.subarray(iv1, iv1 + 3)*/, len1 * invSum);
        Utils::scaleAndAdd(out, out, glm::vec3(vAr[iv2], vAr[iv2 + 1], vAr[iv2 + 2])/*vField.subarray(iv2, iv2 + 3)*/, len2 * invSum);
        Utils::scaleAndAdd(out, out, glm::vec3(vAr[iv3], vAr[iv3 + 1], vAr[iv3 + 2])/*vField.subarray(iv3, iv3 + 3)*/, len3 * invSum);

        if (isQuad)
            Utils::scaleAndAdd(out, out, glm::vec3(vAr[iv4], vAr[iv4 + 1], vAr[iv4 + 2]), len4 * invSum);
        return out;
    }

    void Picking::readAlphas()
    {
        /*
        // check nodejs
        if (!window.module || !window.module.exports) return;
        var fs = eval('require')('fs');
        var path = eval('require')('path');

        var directoryPath = path.join(window.__filename, '../resources/alpha');
        fs.readdir(directoryPath, function(err, files) {
            if (err) return;
            for (var i = 0; i < files.length; ++i) {
                var fname = files[i];
                if (fname == 'square.jpg' || fname == 'skin.jpg') continue;
                Picking.INIT_ALPHAS_NAMES.push(fname);
                Picking.INIT_ALPHAS_PATHS.push(fname);
            }
        });
        */
    }
}