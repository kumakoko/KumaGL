#include "drawables/selection.h"

namespace DigitalSculpt
{
    glm::mat4 _TMP_MATPV = glm::mat4(1.0f);
    glm::mat4 _TMP_MAT = glm::mat4(1.0f);
    glm::vec3 _TMP_VEC = glm::vec3(0.0f);
    glm::vec3 _TMP_AXIS = glm::vec3(0.0f);
    glm::vec3 _BASE = glm::vec3(0.0f, 0.0f, 1.0f);

    float DOT_RADIUS = 50.0f;

    Selection::Selection()
    {
        _circleBuffer = new Buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
        _dotBuffer = new Buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
        _cacheDotMVP = mat4.create();
        _cacheDotSymMVP = mat4.create();
        _cacheCircleMVP = mat4.create();
        _color = glm::vec3(0.8f, 0.0f, 0.0f);
        _offsetX = 0.0f; // horizontal offset (when editing the radius)
        _isEditMode = false;
        init();
    }

    void Selection::init()
    {
        _circleBuffer->update(_getCircleVertices(1.0f));
        _dotBuffer->update(_getDotVertices(0.05f, 10.0f));
    }

    void Selection::release()
    {
        _circleBuffer->release();
        _dotBuffer->release();
    }

    Float32Array Selection::_getCircleVertices(float radius, std::uint32_t nbVertices, bool full)
    {
        float arc = glm::pi<float>() * 2.0f;//Math.PI * 2;

        std::uint32_t start = full ? 1 : 0;
        std::uint32_t end = full ? nbVertices + 2 : nbVertices;
        Float32Array vertices(end * 3, 0.0f);// = new Float32Array(end * 3);

        for (std::uint32_t i = start; i < end; ++i)
        {
            std::uint32_t j = i * 3;
            float segment = static_cast<float>(arc * i) / nbVertices;
            vertices[j + 0] = std::cosf(segment) * radius;
            vertices[j + 1] = std::sinf(segment) * radius;
        }
        return vertices;
    }

    void Selection::_updateMatricesBackground(Camera* camera, Scene* main)
    {
        float screenRadius = main->getSculptManager().getCurrentTool().getScreenRadius();
        float w = camera->_width * 0.5f;
        float h = camera->_height * 0.5f;
        // no need to recompute the ortho proj each time though
        _TMP_MATPV = glm::ortho(-w, w, -h, h, -10.0f, 10.0f);//mat4.ortho(_TMP_MATPV, -w, w, -h, h, -10.0, 10.0);

        _TMP_MAT = glm::identity();// mat4.identity(_TMP_MAT);
        _TMP_VEC.x = -w + main->_mouseX + _offsetX;
        _TMP_VEC.y = h - main->_mouseY;
        _TMP_VEC.z = 0.0f;
        _TMP_MAT = glm::translate(_TMP_MAT, _TMP_VEC);//mat4.translate(_TMP_MAT, _TMP_MAT, vec3.set(_TMP_VEC, -w + main->_mouseX + _offsetX, h - main->_mouseY, 0.0));

        // circle mvp
        _TMP_VEC.x = _TMP_VEC.y = _TMP_VEC.z = screenRadius;
        _cacheCircleMVP = glm::scale(_TMP_MAT, _TMP_VEC);
        //mat4.scale(_cacheCircleMVP, _TMP_MAT, vec3.set(_TMP_VEC, screenRadius, screenRadius, screenRadius));

        _cacheCircleMVP = _TMP_MATPV * _cacheCircleMVP;
        //mat4.mul(_cacheCircleMVP, _TMP_MATPV, _cacheCircleMVP);

        // dot mvp
        _TMP_VEC.x = _TMP_VEC.y = _TMP_VEC.z = DOT_RADIUS;
        _cacheDotMVP = glm::scale(_TMP_MAT, _TMP_VEC);
        //mat4.scale(_cacheDotMVP, _TMP_MAT, vec3.set(_TMP_VEC, DOT_RADIUS, DOT_RADIUS, DOT_RADIUS));
        _cacheDotMVP = _TMP_MATPV * _cacheDotMVP;
        //mat4.mul(_cacheDotMVP, _TMP_MATPV, _cacheDotMVP);
        // symmetry mvp
        _cacheDotSymMVP = _cacheDotSymMVP * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);// mat4.scale(_cacheDotSymMVP, _cacheDotSymMVP, [0.0, 0.0, 0.0]);
    }

    void Selection::_updateMatricesMesh(Camera* camera, Scene* main)
    {
        Picking* picking = main->getPicking();
        Picking* pickingSym = main->getPickingSymmetry();
        float worldRadius = std::sqrt(picking->computeWorldRadius2(true));
        float screenRadius = main->getSculptManager().getCurrentTool().getScreenRadius();

        Mesh* mesh = picking->getMesh();
        float constRadius = DOT_RADIUS * (worldRadius / screenRadius);

        picking->polyLerp(mesh->getNormals(), _TMP_AXIS);

        glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(mesh->getMatrix()));
        _TMP_AXIS = _TMP_AXIS * normalMatrix;
        _TMP_AXIS = glm::normalize(_TMP_AXIS);
        float rad = std::acosf(glm::dot(_BASE, _TMP_AXIS));
        _TMP_AXIS = glm::cross(_BASE, _TMP_AXIS);
        /*
        vec3.transformMat3(_TMP_AXIS, _TMP_AXIS, mat3.normalFromMat4(_TMP_MAT, mesh.getMatrix()));
        vec3.normalize(_TMP_AXIS, _TMP_AXIS);
        var rad = Math.acos(vec3.dot(_BASE, _TMP_AXIS));
        vec3.cross(_TMP_AXIS, _BASE, _TMP_AXIS);
        */

        _TMP_MAT = glm::identity<glm::mat4>();
        _TMP_VEC = mesh->getMatrix() * glm::vec4(picking->getIntersectionPoint(), 1.0f);
        _TMP_MAT = glm::translate(_TMP_MAT, _TMP_VEC);
        _TMP_MAT = glm::rotate(_TMP_MAT, rad, _TMP_AXIS);
        /*
        mat4.identity(_TMP_MAT);
        mat4.translate(_TMP_MAT, _TMP_MAT, vec3.transformMat4(_TMP_VEC, picking.getIntersectionPoint(), mesh.getMatrix()));
        mat4.rotate(_TMP_MAT, _TMP_MAT, rad, _TMP_AXIS);
        */


        _TMP_MATPV = camera->getProjection() * camera->getView();
        //mat4.mul(_TMP_MATPV, camera.getProjection(), camera.getView());

        _TMP_VEC.x = _TMP_VEC.y = _TMP_VEC.z = worldRadius;// , worldRadius, worldRadius)
        _cacheCircleMVP = glm::scale(_TMP_MAT, _TMP_VEC/*glm::vec4(_TMP_VEC, 1.0f)*/);
        _cacheCircleMVP = _TMP_MATPV * _cacheCircleMVP;
        // circle mvp
       /* mat4.scale(_cacheCircleMVP, _TMP_MAT, vec3.set(_TMP_VEC, worldRadius, worldRadius, worldRadius));
        mat4.mul(_cacheCircleMVP, _TMP_MATPV, _cacheCircleMVP);*/


        _TMP_VEC.x = _TMP_VEC.y = _TMP_VEC.z = constRadius;//
        _cacheDotMVP = glm::scale(_TMP_MAT, _TMP_VEC);
        _cacheDotMVP = _TMP_MATPV * _cacheDotMVP;
        // dot mvp
        /*mat4.scale(_cacheDotMVP, _TMP_MAT, vec3.set(_TMP_VEC, constRadius, constRadius, constRadius));
        mat4.mul(_cacheDotMVP, _TMP_MATPV, _cacheDotMVP);*/

        _TMP_VEC = mesh->getMatrix() * glm::vec4(pickingSym->getIntersectionPoint(), 1.0f);
        _TMP_MAT = glm::identity<glm::mat4>();
        _TMP_MAT = glm::translate(_TMP_MAT, _TMP_VEC);
        _TMP_MAT = glm::rotate(_TMP_MAT, rad, _TMP_AXIS);
        // symmetry mvp
      /*  vec3.transformMat4(_TMP_VEC, pickingSym.getIntersectionPoint(), mesh.getMatrix());
        mat4.identity(_TMP_MAT);
        mat4.translate(_TMP_MAT, _TMP_MAT, _TMP_VEC);
        mat4.rotate(_TMP_MAT, _TMP_MAT, rad, _TMP_AXIS);*/

        _TMP_VEC.x = _TMP_VEC.y = _TMP_VEC.z = constRadius;
        _TMP_MAT = glm::scale(_TMP_MAT, _TMP_VEC);
        _cacheDotMVP = _TMP_MATPV * _cacheDotMVP;
        /*mat4.scale(_TMP_MAT, _TMP_MAT, vec3.set(_TMP_VEC, constRadius, constRadius, constRadius));
        mat4.mul(_cacheDotSymMVP, _TMP_MATPV, _TMP_MAT);*/
    }

    void Selection::render(Scene* main)
    {
        // if there's an offset then it means we are editing the tool radius
        //Mesh* pickedMesh = main->getPicking().getMesh() && !_isEditMode;
        Mesh* pickedMesh = nullptr;

        if (!_isEditMode)
            pickedMesh = main->getPicking().getMesh();

        if (pickedMesh!=nullptr)
            _updateMatricesMesh(main->getCamera(), main);
        else
            _updateMatricesBackground(main->getCamera(), main);

        bool drawCircle = main->_action == Enums::Action::NOTHING;
        _color.x = 0.8f;
        _color.y = drawCircle && pickedMesh != nullptr ? 0.0f : 0.4f;
        _color.z = 0.0f;
        //vec3.set(_color, 0.8, drawCircle && pickedMesh ? 0.0 : 0.4, 0.0);
        ShaderLib[Enums::Shader::SELECTION].getOrCreate().draw(this, drawCircle, main->getSculptManager().getSymmetry());
        _isEditMode = false;
    }
}



}