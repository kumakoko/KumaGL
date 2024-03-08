#ifndef digital_sculpt_camera_h__
#define digital_sculpt_camera_h__

#include <functional>
#include <map>

#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4
#include "glm/mat4x4.hpp" // glm::mat4
#include "glm/ext/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale
#include "glm/ext/matrix_clip_space.hpp" // glm::perspective
#include "glm/ext/scalar_constants.hpp" // glm::pi

#include "misc/enums.h"
#include "misc/utils.h"
#include "math3d/geometry.h"

namespace DigitalSculpt
{
    var easeOutQuart = function(r) {
        r = Math.min(1.0, r) - 1.0;
        return -(r * r * r * r - 1.0);
    };

    /*
    extern int DELAY_SNAP;
    extern int DELAY_ROTATE;
    extern int DELAY_TRANSLATE = -1;
    extern int DELAY_MOVE_TO = 200;

    extern glm::vec2 _TMP_VEC2;
    extern glm::vec3 _TMP_VEC3;
    extern glm::vec3 _TMP_VEC3_2;
    extern glm::vec3 _UP;
    extern glm::quat _TMP_QUAT;
    extern glm::mat4 _TMP_MAT;
    extern glm::quat _QUAT_COMP[];
    */

    class Camera
    {
    private:
        //var opts = getOptionsURL();
        Enums::CameraMode _mode;// = opts.cameramode || Enums.CameraMode.ORBIT; // SPHERICAL / PLANE
        Enums::Projection _projectionType;// = opts.projection || Enums::Projection::PERSPECTIVE; // ORTHOGRAPHIC

        glm::quat _quatRot;// = [0.0, 0.0, 0.0, 1.0]; // quaternion rotation
        glm::mat4 _view;// = mat4.create(); // view matrix
        glm::mat4 _proj;// = mat4.create(); // projection matrix
        glm::mat4 _viewport;// = mat4.create(); // viewport matrix

        glm::vec2 _lastNormalizedMouseXY;// = [0.0, 0.0]; // last mouse position ( 0..1 )
        float  _width;// = 0.0; // viewport width
        float _height;// = 0.0; // viewport height

        float _speed;// = 0.0; // solve scale issue
        float _fov;// = Math.min(opts.fov, 150); // vertical field of view

        // translation stuffs
        glm::vec3 _trans;// = [0.0, 0.0, 30.0];
        float _moveX = 0; // free look (strafe), possible values : -1, 0, 1
        float _moveZ = 0; // free look (strafe), possible values : -1, 0, 1

        // pivot stuffs
        bool _usePivot;// = opts.pivot; // if rotation is centered around the picked point
        glm::vec3 _center;// = [0.0, 0.0, 0.0]; // center of rotation
        glm::vec3 _offset;// = [0.0, 0.0, 0.0];

        // orbit camera
        float _rotX = 0.0f; // x rot for orbit camera
        float _rotY = 0.0f; // y rot for orbit camera

        // near far
        float _near = 0.05f;
        float _far = 5000.0f;

        SixElemArray _lastBBox;

        //_timers = {}; // animation timers
        std::map<std::string, int> _timers;
    public:
        /******************************************************************************************************************
         * Desc:
         * Method:    Camera
         * Returns:
         * Parameter: Scene * main
         ****************************************************************************************************************/
        Camera(Scene* main);

        /******************************************************************************************************************
         * Desc:
         * Method:    setProjectionType
         * Returns:   void
         * Parameter: Enums::Projection type
         ****************************************************************************************************************/
        void setProjectionType(Enums::Projection type);

        /******************************************************************************************************************
         * Desc:
         * Method:    setMode
         * Returns:   void
         * Parameter: Enums::CameraMode mode
         ****************************************************************************************************************/
        void setMode(Enums::CameraMode mode);

        /******************************************************************************************************************
         * Desc:
         * Method:    setFov
         * Returns:   void
         * Parameter: float fov
         ****************************************************************************************************************/
        void setFov(float fov);

        /******************************************************************************************************************
         * Desc:
         * Method:    setUsePivot
         * Returns:   void
         * Parameter: bool b
         ****************************************************************************************************************/
        inline void setUsePivot(bool b)
        {
            _usePivot = b;
        }

        /******************************************************************************************************************
         * Desc:
         * Method:    toggleUsePivot
         * Returns:   void
         ****************************************************************************************************************/
        inline void toggleUsePivot()
        {
            _usePivot = !_usePivot;
        }

        /******************************************************************************************************************
         * Desc:
         * Method:    getView
         * Returns:   glm::mat4&
         ****************************************************************************************************************/
        inline glm::mat4& getView()
        {
            return _view;
        }

        /******************************************************************************************************************
         * Desc:
         * Method:    getView
         * Returns:   const glm::mat4&
         ****************************************************************************************************************/
        inline const glm::mat4& getView() const
        {
            return _view;
        }

        /******************************************************************************************************************
         * Desc:
         * Method:    getProjection
         * Returns:   glm::mat4&
         ****************************************************************************************************************/
        inline glm::mat4& getProjection()
        {
            return _proj;
        }

        /******************************************************************************************************************
         * Desc:
         * Method:    getProjection
         * Returns:   const glm::mat4&
         ****************************************************************************************************************/
        inline const glm::mat4& getProjection() const
        {
            return _proj;
        }

        /******************************************************************************************************************
         * Desc:
         * Method:    getProjectionType
         * Returns:   DigitalSculpt::Enums::Projection
         ****************************************************************************************************************/
        inline Enums::Projection getProjectionType() const
        {
            return _projectionType;
        }

        /******************************************************************************************************************
         * Desc:
         * Method:    isOrthographic
         * Returns:   bool
         ****************************************************************************************************************/
        inline bool isOrthographic() const
        {
            return _projectionType == Enums::Projection::ORTHOGRAPHIC;
        }

        /******************************************************************************************************************
         * Desc:
         * Method:    getMode
         * Returns:   DigitalSculpt::Enums::CameraMode
         ****************************************************************************************************************/
        inline Enums::CameraMode getMode() const
        {
            return _mode;
        }

        /******************************************************************************************************************
         * Desc:
         * Method:    getFov
         * Returns:   float
         ****************************************************************************************************************/
        inline float getFov() const
        {
            return _fov;
        }

        /******************************************************************************************************************
         * Desc:
         * Method:    getUsePivot
         * Returns:   bool
         ****************************************************************************************************************/
        inline bool getUsePivot() const
        {
            return _usePivot;
        }

        /******************************************************************************************************************
         * Desc:
         * Method:    getConstantScreen
         * Returns:   float
         ****************************************************************************************************************/
        float getConstantScreen() const;

        /******************************************************************************************************************
         * Desc:
         * Method:    start
         * Returns:   void
         * Parameter: float mouseX
         * Parameter: float mouseY
         ****************************************************************************************************************/
        void start(float mouseX, float mouseY);

        /******************************************************************************************************************
         * Desc:
         * Method:    setPivot
         * Returns:   void
         * Parameter: const glm::vec3 & pivot
         ****************************************************************************************************************/
        void setPivot(const glm::vec3& pivot);

        /******************************************************************************************************************
         * Desc: Compute rotation values (by updating the quaternion)
         * Method:    rotate
         * Returns:   void
         * Parameter: float mouseX
         * Parameter: float mouseY
         ****************************************************************************************************************/
        void rotate(float mouseX, float mouseY);

        /******************************************************************************************************************
         * Desc:
         * Method:    setOrbit
         * Returns:   void
         * Parameter: float rx
         * Parameter: float ry
         ****************************************************************************************************************/
        void setOrbit(float rx, float ry);

        inline float getTransZ() const
        {
            return _projectionType == Enums::Projection::PERSPECTIVE ? _trans.z * 45.0f / _fov : 1000.0f;
        }

        void updateView();

        void optimizeNearFar(SixElemArray* bb);

        void updateProjection();

        void updateTranslation();

        void translate(float dx, float dy);

        void zoom(float df);

        void setAndFocusOnPivot(const glm::vec3& pivot, float zoom);

        void moveToDelay(float x, float y, float z);

        void setTrans(const glm::vec3& trans);

        inline float getOrthoZoom() const
        {
            return std::fabs(_trans.z/*[2]*/) * 0.00055f;
        }

        void updateOrtho();

        glm::vec3* computePosition(glm::vec3* out);

        void resetView();

        void resetViewFront();

        void resetViewBack();

        void resetViewTop();

        void resetViewBottom();

        void resetViewLeft();

        void resetViewRight();

        void toggleViewFront();

        void toggleViewTop();

        void toggleViewLeft();

        glm::mat4* computeWorldToScreenMatrix(glm::mat4* mat);

        /******************************************************************************************************************
         * Desc: Project the mouse coordinate into the world coordinate at a given z
         * Method:    unproject
         * Returns:   glm::vec3
         * Parameter: float mouseX
         * Parameter: float mouseY
         * Parameter: float z
         ****************************************************************************************************************/
        glm::vec3 unproject(float mouseX, float mouseY, float z);


        /******************************************************************************************************************
         * Desc: Project a vertex onto the screen
         * Method:    project
         * Returns:   glm::vec3
         * Parameter: const glm::vec3 & pos
         ****************************************************************************************************************/
        glm::vec3 project(const glm::vec3& pos);

        /******************************************************************************************************************
         * Desc:
         * Method:    onResize
         * Returns:   void
         * Parameter: float width
         * Parameter: float height
         ****************************************************************************************************************/
        void onResize(float width, float height);

        /******************************************************************************************************************
         * Desc:
         * Method:    snapClosestRotation
         * Returns:   void
         ****************************************************************************************************************/
        void snapClosestRotation();

        /******************************************************************************************************************
         * Desc:
         * Method:    clearTimerN
         * Returns:   void
         * Parameter: int n
         ****************************************************************************************************************/
        void clearTimerN(int n);

        /******************************************************************************************************************
         * Desc:
         * Method:    delay
         * Returns:   void
         * Parameter: std::function<void
         * Parameter: float
         * Parameter: float
         * Parameter: > cb
         * Parameter: float duration
         * Parameter: const std::string & name
         ****************************************************************************************************************/
        void delay(std::function<void(float, float)> cb, float duration, const std::string& name);

        void _translateDelta(const glm::vec3& delta, float dr);

        void translateDelay(const glm::vec3& delta, float duration);

        void _rotDelta(const float* delta, float dr);

        void rotateDelay(const float* delta, float duration);

        void _quatDelta(const glm::quat& qDelta, float dr);

        void quatDelay(const glm::quat& target, float duration);

        void _centerDelta(const float* delta, float dr);

        void centerDelay(const glm::vec3& target, float duration);

        void _offsetDelta(const glm::vec3& delta, float dr);

        void offsetDelay(const glm::vec3& target, float duration);

        float computeFrustumFit();
    };
}

#endif // digital_sculpt_camera_h__
