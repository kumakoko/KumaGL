#include <utility>
#include "math3d/camera.h"
#include "math3d/picking.h"
#include "math3d/glm_extension.h"
#include "glm/ext/quaternion_float.hpp"


namespace DigitalSculpt
{
    static int DELAY_SNAP = 200;
    static int DELAY_ROTATE = -1;
    static int DELAY_TRANSLATE = -1;
    static int DELAY_MOVE_TO = 200;

    static glm::vec2 _TMP_VEC2 = glm::vec2(0.0f, 0.0f);
    static glm::vec3 _TMP_VEC3 = glm::vec3(0.0f, 0.0f, 0.0f);
    static glm::vec3 _TMP_VEC3_2 = glm::vec3(0.0f, 0.0f, 0.0f);
    static glm::vec3 _UP = glm::vec3(0.0f, 1.0f, 0.0f);
    static glm::quat _TMP_QUAT = glm::quat(0.0f, 0.0f, 0.0f, 1.0f);
    static glm::mat4 _TMP_MAT = glm::identity<glm::mat4>();

    static glm::quat _QUAT_COMP[] =
    {
        glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
        glm::quat(0.0f, 1.0f, 0.0f, 0.0f),
        glm::quat(0.0f, 0.0f, 1.0f, 0.0f),
        glm::quat(0.0f, 0.0f, 0.0f, 1.0f),
        glm::quat(Utils::SQRT1_2, Utils::SQRT1_2, 0.0f, 0.0f),
        glm::quat(Utils::SQRT1_2, -Utils::SQRT1_2, 0.0f, 0.0f),
        glm::quat(Utils::SQRT1_2, 0, Utils::SQRT1_2, 0.0f),
        glm::quat(Utils::SQRT1_2, 0, -Utils::SQRT1_2, 0.0f),
        glm::quat(Utils::SQRT1_2, 0, 0, Utils::SQRT1_2),
        glm::quat(Utils::SQRT1_2, 0, 0, -Utils::SQRT1_2),
        glm::quat(0.0f, Utils::SQRT1_2, Utils::SQRT1_2, 0.0f),
        glm::quat(0.0f, Utils::SQRT1_2, -Utils::SQRT1_2, 0.0f),
        glm::quat(0.0f, Utils::SQRT1_2, 0.0f, Utils::SQRT1_2),
        glm::quat(0.0f, Utils::SQRT1_2, 0.0f, -Utils::SQRT1_2),
        glm::quat(0.0f, 0.0f, Utils::SQRT1_2, Utils::SQRT1_2),
        glm::quat(0.0f, 0.0f, Utils::SQRT1_2, -Utils::SQRT1_2),
        glm::quat(0.5f, 0.5f, 0.5f, 0.5f),
        glm::quat(0.5f, 0.5f, 0.5f, -0.5f),
        glm::quat(0.5f, 0.5f, -0.5f, 0.5f),
        glm::quat(0.5f, 0.5f, -0.5f, -0.5f),
        glm::quat(0.5f, -0.5f, 0.5f, 0.5f),
        glm::quat(0.5f, -0.5f, 0.5f, -0.5f),
        glm::quat(0.5f, -0.5f, -0.5f, 0.5f),
        glm::quat(-0.5f, 0.5f, 0.5f, 0.5f),
    };

    Camera::Camera(Scene* main)
    {
        _main = main;

        var opts = getOptionsURL();
        _mode = opts.cameramode || Enums::CameraMode::ORBIT; // SPHERICAL / PLANE
        _projectionType = opts.projection || Enums::Projection::PERSPECTIVE; // ORTHOGRAPHIC

        _quatRot = glm::quat(0.0f, 0.0f, 0.0f, 1.0f); // quaternion rotation
        _view = glm::identity<glm::mat4>();// mat4.create(); // view matrix
        _proj = glm::identity<glm::mat4>();// mat4.create(); // projection matrix
        _viewport = glm::identity<glm::mat4>(); //mat4.create(); // viewport matrix

        _lastNormalizedMouseXY = glm::vec2(0.0f, 0.0f); // last mouse position ( 0..1 )
        _width = 0.0f; // viewport width
        _height = 0.0f; // viewport height

        _speed = 0.0f; // solve scale issue
        _fov = std::min(opts.fov, 150.0f); // vertical field of view

        // translation stuffs
        _trans = glm::vec3(0.0f, 0.0f, 30.0f);
        _moveX = 0; // free look (strafe), possible values : -1, 0, 1
        _moveZ = 0; // free look (strafe), possible values : -1, 0, 1

        // pivot stuffs
        _usePivot = opts.pivot; // if rotation is centered around the picked point
        _center = glm::vec3(0.0f);// [0.0, 0.0, 0.0] ; // center of rotation
        _offset = glm::vec3(0.0f);//[0.0, 0.0, 0.0];

        // orbit camera
        _rotX = 0.0f; // x rot for orbit camera
        _rotY = 0.0f; // y rot for orbit camera

        // near far
        _near = 0.05f;
        _far = 5000.0f;

        //_timers = {}; // animation timers

        resetView();
    }

    void Camera::setProjectionType(Enums::Projection type)
    {
        _projectionType = type;
        updateProjection();
        updateView();
    }

    void Camera::setMode(Enums::CameraMode mode)
    {
        _mode = mode;
        if (mode == Enums::CameraMode::ORBIT)
            resetViewFront();
    }

    void Camera::setFov(float fov)
    {
        _fov = fov;
        updateView();
        optimizeNearFar();
    }

    float Camera::getConstantScreen() const
    {
        float cwidth = _main->getCanvas().clientWidth;
        if (_projectionType == Enums::Projection::ORTHOGRAPHIC)
            return cwidth / getOrthoZoom();
        return cwidth * _proj[0];
    }

    void Camera::start(float mouseX, float mouseY)
    {
        _lastNormalizedMouseXY = Geometry::normalizedMouse(mouseX, mouseY, _width, _height);

        if (!_usePivot)
            return;
        //var main = _main;
        Picking* picking = main->getPicking();
        picking->intersectionMouseMeshes(main.getMeshes(), mouseX, mouseY);

        if (picking->getMesh() != nullptr)
        {
            _TMP_VEC3 = picking->getIntersectionPoint() * picking->getMesh()->getMatrix();
            setPivot(_TMP_VEC3);
            //vec3.transformMat4(_TMP_VEC3, picking.getIntersectionPoint(), picking.getMesh().getMatrix());
            //setPivot(_TMP_VEC3);
        }
    }

    void  Camera::setPivot(const glm::vec3& pivot)
    {
        _TMP_QUAT = glm::inverse(_quatRot);
        _offset = _TMP_QUAT * _offset - _center;
        _center = pivot;
        _offset = _TMP_QUAT * (_offset + _center);

        /*
        vec3.transformQuat(_offset, _offset, quat.invert(_TMP_QUAT, _quatRot));
        vec3.sub(_offset, _offset, _center);

        // set new pivot
        vec3.copy(_center, pivot);
        vec3.add(_offset, _offset, _center);
        vec3.transformQuat(_offset, _offset, _quatRot);
        */

        // adjust zoom
        if (_projectionType == Enums::Projection::PERSPECTIVE)
        {
            float oldZoom = getTransZ();
            _trans.z/*[2]*/ = glm::distance(computePosition(), _center) * _fov / 45.0f;//vec3.dist(computePosition(), _center) * _fov / 45;
            _offset.z/*[2]*/ += getTransZ() - oldZoom;
        }
        else
        {
            _offset.z/*[2]*/ = 0.0f;
        }
    }

    void Camera::rotate(float mouseX, float mouseY)
    {
        glm::vec3& axisRot = _TMP_VEC3;
        glm::vec2& diff = _TMP_VEC2;

        glm::vec2 normalizedMouseXY = Geometry::normalizedMouse(mouseX, mouseY, _width, _height);

        if (_mode == Enums::CameraMode::ORBIT)
        {
            diff = normalizedMouseXY - _lastNormalizedMouseXY; //vec2.sub(diff, normalizedMouseXY, _lastNormalizedMouseXY);
            setOrbit(_rotX - diff.y/*[1]*/ * 2.0f, _rotY + diff.x/*[0]*/ * 2.0f);
            rotateDelay(glm::vec2(-diff.y/*[1]*/ * 6.0f, diff.x/*[0]*/ * 6.0f), DELAY_ROTATE);
        }
        else if (_mode == Enums::CameraMode::PLANE)
        {
            float length = glm::distance(_lastNormalizedMouseXY, normalizedMouseXY);//vec2.dist(_lastNormalizedMouseXY, normalizedMouseXY);
            diff = normalizedMouseXY - _lastNormalizedMouseXY;//vec2.sub(diff, normalizedMouseXY, _lastNormalizedMouseXY);
            axisRot = glm::normalize(glm::vec3(-diff.y, diff.x, 0.0f));//vec3.normalize(axisRot, vec3.set(axisRot, -diff[1], diff[0], 0.0));
            _TMP_QUAT = glm::angleAxis(length * 2.0f, axisRot);//quat.mul(_quatRot, quat.setAxisAngle(_TMP_QUAT, axisRot, length * 2.0), _quatRot);
            _quatRot = _TMP_QUAT * _quatRot;
            rotateDelay(glm::quat(axisRot.x, axisRot.y, axisRot.z, length * 6.0f), DELAY_ROTATE);//rotateDelay([axisRot[0], axisRot[1], axisRot[2], length * 6], DELAY_ROTATE);

        }
        else if (_mode == Enums::CameraMode::SPHERICAL)
        {
            glm::vec3 mouseOnSphereBefore = Geometry::mouseOnUnitSphere(_lastNormalizedMouseXY);
            glm::vec3 mouseOnSphereAfter = Geometry::mouseOnUnitSphere(normalizedMouseXY);
            float angle = std::acosf(std::min(1.0f, glm::dot(mouseOnSphereBefore, mouseOnSphereAfter)));
            axisRot = glm::normalize(glm::cross(mouseOnSphereBefore, mouseOnSphereAfter));//vec3.normalize(axisRot, vec3.cross(axisRot, mouseOnSphereBefore, mouseOnSphereAfter));
            _TMP_QUAT = glm::angleAxis(angle * 2.0f, axisRot);////quat.mul(_quatRot, quat.setAxisAngle(_TMP_QUAT, axisRot, angle * 2.0), _quatRot);
            _quatRot = _TMP_QUAT * _quatRot;
            rotateDelay(glm::quat(axisRot.x, axisRot.y, axisRot.z, angle * 6.0f), DELAY_ROTATE);
        }

        _lastNormalizedMouseXY = normalizedMouseXY;
        updateView();
    }

    void Camera::setOrbit(float rx, float ry)
    {
        float radLimit = glm::pi<float>() * 0.49f;
        float _min = std::min(rx, radLimit);
        _rotX = std::max(_min, -radLimit);
        _rotY = ry;
        //var qrt = _quatRot;
        _quatRot = glm::identity<glm::quat>();//quat.identity(qrt);
        GlmExtension::rotateX(_quatRot, _quatRot, _rotX);//quat.rotateX(qrt, qrt, _rotX);
        GlmExtension::rotateY(_quatRot, _quatRot, _rotY);//quat.rotateY(qrt, qrt, _rotY);
    }

    void Camera::updateView()
    {
        glm::vec3& center = _TMP_VEC3;

        //var view = _view;
        float tx = _trans.x;
        float ty = _trans.y;

        //var off = _offset;
        _TMP_VEC3_2.x = tx - _offset.x;//vec3.set(_TMP_VEC3_2, tx - off[0], ty - off[1], getTransZ() - off[2]);
        _TMP_VEC3_2.y = ty - _offset.y;
        _TMP_VEC3_2.z = getTransZ() - _offset.z;

        center.x = tx - _offset.x;//vec3.set(center, tx - off[0], ty - off[1], -off[2]);
        center.y = ty - _offset.y;
        center.z = -_offset.z;

        _view = glm::lookAt(_TMP_VEC3_2, center, _UP);//mat4.lookAt(view, _TMP_VEC3_2, center, _UP);

        _TMP_MAT = glm::mat4_cast(_quatRot);//mat4.mul(view, view, mat4.fromQuat(_TMP_MAT, _quatRot));
        _view = _TMP_MAT * _view;
        _view = glm::translate(_view, GlmExtension::negate(_TMP_VEC3, _center));//mat4.translate(view, view, vec3.negate(_TMP_VEC3, _center));
    }

    void Camera::optimizeNearFar(SixElemArray* bb)
    {
        if (bb == nullptr)
            bb = &_lastBBox;
        /*
        if (!bb) bb = _lastBBox;
        if (!bb) return;
        _lastBBox = bb;
        */

        glm::vec3 eye = computePosition(_TMP_VEC3_2);
        _TMP_VEC3.x = (bb->at(0) + bb->at(3)) * 0.5f;//var boxCenter = vec3.set(_TMP_VEC3, (bb[0] + bb[3]) * 0.5, (bb[1] + bb[4]) * 0.5, (bb[2] + bb[5]) * 0.5);
        _TMP_VEC3.y = (bb->at(1) + bb->at(4)) * 0.5f;
        _TMP_VEC3.z = (bb->at(2) + bb->at(5)) * 0.5f;
        float distToBoxCenter = glm::distance(eye, _TMP_VEC3);//vec3.dist(eye, boxCenter);

        _TMP_VEC3.x = bb->at(3);
        _TMP_VEC3.y = bb->at(4);
        _TMP_VEC3.z = bb->at(5);
        float boxRadius = 0.5f * glm::distance(eye, _TMP_VEC3);//vec3.dist(bb, vec3.set(_TMP_VEC3, bb[3], bb[4], bb[5]));

        _near = std::max(0.01f, distToBoxCenter - boxRadius);
        _far = boxRadius + distToBoxCenter;
        updateProjection();
    }

    void Camera::updateProjection()
    {
        if (_projectionType == Enums::Projection::PERSPECTIVE)
        {
            _proj = glm::perspective(_fov * glm::pi<float>() / 180.0f, _width / _height, _near, _far);//mat4.perspective(_proj, _fov * Math.PI / 180.0, _width / _height, _near, _far);
            float* proj_ptr = glm::value_ptr(_proj);
            proj_ptr[10] = -1.0f;
            proj_ptr[14] = -2.0f * _near;
        }
        else
        {
            updateOrtho();
        }
    }

    void Camera::updateTranslation()
    {
        //var trans = _trans;
        _trans.x/*[0]*/ += _moveX * _speed * _trans.z/*[2]*/ / 50.0f / 400.0f;
        _trans.z/*[2]*/ = std::max(0.00001f, _trans.z/*[2]*/ + _moveZ * _speed / 400.0f);
        if (_projectionType == Enums::Projection::ORTHOGRAPHIC)
            updateOrtho();
        updateView();
    }

    void Camera::translate(float dx, float dy)
    {
        float factor = _speed * _trans.z/*[2]*/ / 54.0f;
        glm::vec3 delta = glm::vec3(-dx * factor, dy * factor, 0.0f);
        setTrans((_trans += delta));//setTrans(vec3.add(_trans, _trans, delta));
        delta *= 5.0f;//vec3.scale(delta, delta, 5);
        translateDelay(delta, DELAY_TRANSLATE);
    }

    void Camera::zoom(float df)
    {
        //var delta = [0.0, 0.0, 0.0];
        //vec3.sub(delta, _offset, _trans);
        //vec3.scale(delta, delta, df * _speed / 54);
        glm::vec3 delta = (_offset - _trans) * df * _speed / 54.0f;

        if (df < 0.0f)
            delta.x/*[0]*/ = delta.y/*[1]*/ = 0.0f;

        setTrans(_trans += delta);//setTrans(vec3.add(_trans, _trans, delta));
        delta *= 5.0f;// vec3.scale(delta, delta, 5);
        translateDelay(delta, DELAY_TRANSLATE);
    }

    void Camera::setAndFocusOnPivot(const glm::vec3& pivot, float zoom)
    {
        setPivot(pivot);
        moveToDelay(_offset[0], _offset[1], _offset[2] + zoom);
    }

    void Camera::moveToDelay(float x, float y, float z)
    {
        //var delta = [x, y, z];
        //translateDelay(vec3.sub(delta, delta, _trans), DELAY_MOVE_TO);
        glm::vec3 delta = glm::vec3(x, y, z) - _trans;
        translateDelay(delta, DELAY_MOVE_TO);
    }

    void Camera::setTrans(const glm::vec3& trans)
    {
        _trans = trans;// vec3.copy(_trans, trans);
        if (_projectionType == Enums::Projection::ORTHOGRAPHIC)
            updateOrtho();
        updateView();
    }

    void Camera::updateOrtho()
    {
        float delta = getOrthoZoom();
        float w = _width * delta;
        float h = _height * delta;
        _proj = glm::ortho(-w, w, -h, h, -_near, _far);
    }

    glm::vec3* Camera::computePosition(glm::vec3* out)
    {
        if (nullptr == out)
            out = new glm::vec3(0.0f);
        //out = out || [0, 0, 0];

        float* view = glm::value_ptr(_view);
        out->x = -view[12];//vec3.set(out, -view[12], -view[13], -view[14]);
        out->x = -view[13];
        out->x = -view[14];

        glm::mat3 m3 = glm::transpose(glm::mat3(_view)); //mat3.fromMat4(_TMP_MAT, view);
        //mat3.transpose(_TMP_MAT, _TMP_MAT);
        *out = m3 * (*out); //return vec3.transformMat3(out, out, _TMP_MAT);
        return out;
    }

    void Camera::resetView()
    {
        _speed = Utils::SCALE * 1.5f;
        glm::vec3 zero(0.0f);
        centerDelay(zero, DELAY_MOVE_TO);
        offsetDelay(zero, DELAY_MOVE_TO);
        auto delta = glm::vec3(0.0f, 0.0f, 30.0f + _speed / 3.0);
        delta -= _trans;//vec3.sub(delta, delta, _trans);
        translateDelay(delta, DELAY_MOVE_TO);
        quatDelay(glm::quat(0.0f, 0.0f, 0.0f, 1.0f), DELAY_MOVE_TO);
    }

    void Camera::resetViewFront()
    {
        quatDelay(glm::quat(0.0f, 0.0f, 0.0f, 1.0f), DELAY_SNAP);
    }

    void Camera::resetViewBack()
    {
        quatDelay(glm::quat(0.0f, 1.0f, 0.0f, 0.0f), DELAY_SNAP);
    }

    void Camera::resetViewTop()
    {
        quatDelay(glm::quat(Utils::SQRT1_2, 0.0, 0.0, Utils::SQRT1_2), DELAY_SNAP);
    }

    void Camera::resetViewBottom()
    {
        quatDelay(glm::quat(-Utils::SQRT1_2, 0.0f, 0.0f, Utils::SQRT1_2), DELAY_SNAP);
    }

    void Camera::resetViewLeft() {
        quatDelay(glm::quat(0.0f, -Utils::SQRT1_2, 0.0f, Utils::SQRT1_2), DELAY_SNAP);
    }

    void Camera::resetViewRight()
    {
        quatDelay(glm::quat(0.0f, Utils::SQRT1_2, 0.0f, Utils::SQRT1_2), DELAY_SNAP);
    }

    void Camera::toggleViewFront()
    {
        if (Math.abs(_quatRot[3]) > 0.99f) resetViewBack();
        else resetViewFront();
    }

    void Camera::toggleViewTop()
    {
        float dot = _quatRot[0] * Utils::SQRT1_2 + _quatRot[3] * Utils::SQRT1_2;
        if (dot * dot > 0.99f)
            resetViewBottom();
        else
            resetViewTop();
    }

    void Camera::toggleViewLeft()
    {
        float dot = -_quatRot[1] * Utils::SQRT1_2 + _quatRot[3] * Utils::SQRT1_2;
        if (dot * dot > 0.99f)
            resetViewRight();
        else
            resetViewLeft();
    }


    glm::mat4* Camera::computeWorldToScreenMatrix(glm::mat4* mat)
    {
        if (nullptr == mat)
            mat = new glm::mat4(1.0f);

        //return mat4.mul(mat, mat4.mul(mat, _viewport, _proj), _view);
        *mat = _viewport * _proj * _view;
        return mat;
    }

    glm::vec3 Camera::unproject(float mouseX, float mouseY, float z)
    {
        glm::vec3 out(0.0f);//var out = [0.0, 0.0, 0.0];
        _TMP_MAT = glm::inverse(*computeWorldToScreenMatrix(&_TMP_MAT));//mat4.invert(_TMP_MAT, computeWorldToScreenMatrix(_TMP_MAT));
        return _TMP_MAT * glm::vec4(GlmExtension::set(out, mouseX, _height - mouseY, z), 1.0f);//return vec3.transformMat4(out, vec3.set(out, mouseX, _height - mouseY, z), _TMP_MAT);
    }

    glm::vec3 Camera::project(const glm::vec3& pos)
    {
        glm::vec3 out(0.0f);//var out = [0.0, 0.0, 0.0];
        out = *computeWorldToScreenMatrix(&_TMP_MAT) * glm::vec4(pos, 1.0f);//vec3.transformMat4(out, pos, computeWorldToScreenMatrix(_TMP_MAT));
        out.y/*[1]*/ = _height - out.y/*[1]*/;
        return out;
    }

    void Camera::onResize(float width, float height)
    {
        _width = width;
        _height = height;

        //var vp = _viewport;
        //mat4.identity(vp);
        _viewport = glm::scale(glm::identity<glm::mat4>(), GlmExtension::set(_TMP_VEC3, 0.5f * width, 0.5f * height, 0.5f));//mat4.scale(vp, vp, vec3.set(_TMP_VEC3, 0.5 * width, 0.5 * height, 0.5));
        _viewport = glm::translate(_viewport, GlmExtension::set(_TMP_VEC3, 1.0f, 1.0f, 1.0f));//mat4.translate(vp, vp, vec3.set(_TMP_VEC3, 1.0, 1.0, 1.0));

        updateProjection();
    }

    void Camera::snapClosestRotation()
    {
        //var qrot = _quatRot;
        float min = std::numeric_limits<float>::infinity();//var min = Infinity;
        int id = 0;
        std::size_t nbQComp = Utils::GetArrayElementsCount(_QUAT_COMP);// .length;

        for (std::size_t i = 0; i < nbQComp; ++i)
        {
            float dot = glm::dot(_quatRot, _QUAT_COMP[i]);//var dot = quat.dot(qrot, _QUAT_COMP[i]);
            dot = 1 - dot * dot;
            if (min < dot)
                continue;
            min = dot;
            id = i;
        }

        quatDelay(_QUAT_COMP[id], DELAY_SNAP);
    }

    void Camera::clearTimerN(int n)
    {
        /*暂时注解
        window.clearInterval(_timers[n]);
        _timers[n] = 0;
        */
    }

    void Camera::_translateDelta(const glm::vec3& delta, float dr)
    {
        //var trans = _trans;
        GlmExtension::scaleAndAdd(_trans, _trans, glm::vec3(delta), dr);//vec3.scaleAndAdd(trans, trans, delta, dr);
        setTrans(_trans);
        _main->render();
    }

    void Camera::translateDelay(const glm::vec3& delta, float duration)
    {
        auto iter = _timers.find("translate");
        if (iter != _timers.end())
            clearTimerN(iter->second);

        /*
        if (duration == = 0.0)
            return cb(1.0);
        else if (duration < 0.0)
            return;
        */

        /*暂时先注释
        var lastR = 0;
        var tStart = (new Date()).getTime();
        _timers[nTimer] = window.setInterval(function() {
            var r = ((new Date()).getTime() - tStart) / duration;
            r = easeOutQuart(r);
            cb(r - lastR, r);
            lastR = r;
            if (r >= 1.0)
                clearTimerN(nTimer);
        }.bind(this), 16.6);
        */
    }

    void Camera::_rotDelta(const float* delta, float dr)
    {
        if (_mode == Enums::CameraMode::ORBIT)
        {
            float rx = _rotX + delta[0] * dr;
            float ry = _rotY + delta[1] * dr;
            setOrbit(rx, ry);
        }
        else
        {
            _TMP_QUAT = glm::angleAxis(delta[3] * dr, glm::vec3(delta[0], delta[1], delta[2]));
            _quatRot = _TMP_QUAT * _quatRot;
            //quat.mul(_quatRot, quat.setAxisAngle(_TMP_QUAT, delta, delta[3] * dr), _quatRot);
        }

        updateView();
        _main->render();
    }

    void Camera::rotateDelay(const float* delta, float duration)
    {
        auto iter = _timers.find("rotate");
        if (iter != _timers.end())
            clearTimerN(iter->second);

        /*
        if (duration == = 0.0)
            return cb(1.0);
        else if (duration < 0.0)
            return;
        */

        /*暂时先注释
        var lastR = 0;
        var tStart = (new Date()).getTime();
        _timers[nTimer] = window.setInterval(function() {
            var r = ((new Date()).getTime() - tStart) / duration;
            r = easeOutQuart(r);
            cb(r - lastR, r);
            lastR = r;
            if (r >= 1.0)
                clearTimerN(nTimer);
        }.bind(this), 16.6);
        */
    }

    //glm::quat_cast()
    void Camera::_quatDelta(const glm::quat& qDelta, float dr)
    {
        // quat.identity(_TMP_QUAT);
        _TMP_QUAT = glm::slerp(glm::identity<glm::quat>(), qDelta, dr);//quat.slerp(_TMP_QUAT, _TMP_QUAT, qDelta, dr);
        //var qrt = _quatRot;
        _quatRot = _TMP_QUAT * _quatRot;//quat.mul(_quatRot, _quatRot, _TMP_QUAT);

        if (_mode == Enums.CameraMode.ORBIT)
        {
            float qx = _quatRot[0];
            float qy = _quatRot[1];
            float qz = _quatRot[2];
            float qw = _quatRot[3];
            // find back euler values
            _rotY = std::atan2f(2 * (qw * qy + qz * qx), 1 - 2 * (qy * qy + qz * qz));
            _rotX = std::atan2f(2 * (qw * qx + qy * qz), 1 - 2 * (qz * qz + qx * qx));
        }

        updateView();
        _main->render();
    }

    void Camera::quatDelay(const glm::quat& target, float duration)
    {
        //var qDelta = [0.0, 0.0, 0.0, 0.0];
        glm::quat qDelta = glm::conjugate(_quatRot);//quat.conjugate(qDelta, _quatRot);
        qDelta *= target;//quat.mul(qDelta, qDelta, target);
        qDelta = glm::normalize(qDelta);//quat.normalize(qDelta, qDelta);

        //var cb = _quatDelta.bind(this, qDelta);

        // 暂时注解
        //delay(cb, duration, 'quat');
    }

    void Camera::_centerDelta(const float* delta, float dr)
    {
        GlmExtension::scaleAndAdd(_center, _center, glm::vec3(delta[0], delta[1], delta[2]), dr);//vec3.scaleAndAdd(_center, _center, delta, dr);
        updateView();
        _main->render();
    }

    void Camera::centerDelay(const glm::vec3& target, float duration)
    {
        //var delta = [0.0, 0.0, 0.0];
        glm::vec3 delta = target - _center;
        //var cb = _centerDelta.bind(this, delta);
        //delay(cb, duration, 'center');
    }

    void Camera::_offsetDelta(const glm::vec3& delta, float dr)
    {
        GlmExtension::scaleAndAdd(_offset, _offset, delta, dr);// .scaleAndAdd(_offset, _offset, delta, dr);
        updateView();
        _main->render();
    }

    void Camera::offsetDelay(const glm::vec3& target, float duration)
    {
        //var delta = [0.0, 0.0, 0.0];
        //vec3.sub(delta, target, _offset);
        glm::vec3 delta = target - _offset;
        //var cb = _offsetDelta.bind(this, delta);
        //delay(cb, duration, 'offset');
    }

    float Camera::computeFrustumFit()
    {
        //var near = _near;
        float x;

        if (_projectionType == Enums::Projection::ORTHOGRAPHIC)
        {
            x = Utils::min<float>(_width, _height) / _near * 0.5f;
            return std::sqrt(1.0f + x * x) / x;
        }

        const float* proj = glm::value_ptr(_proj);
        float left = _near * (proj[8] - 1.0f) / proj[0];
        float right = _near * (1.0f + proj[8]) / proj[0];
        float top = _near * (1.0f + proj[9]) / proj[5];
        float bottom = _near * (proj[9] - 1.0f) / proj[5];
        float vertical2 = std::fabs(right - left);
        float horizontal2 = std::fabs(top - bottom);

        x = Utils::min<float>(horizontal2, vertical2) / _near * 0.5f;
        return (_fov / 45.0f) * std::sqrt(1.0f + x * x) / x;
    }
}