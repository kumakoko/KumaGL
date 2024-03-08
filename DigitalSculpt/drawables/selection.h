#ifndef digital_sculpt_selection_h__
#define digital_sculpt_selection_h__

#include <cmath>
#include <limits>
#include "misc/enums.h"
#include "render/buffer.h"
#include "render/shaderlib.h"
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
    extern glm::mat4 _TMP_MATPV;
    extern glm::mat4 _TMP_MAT;
    extern glm::vec3 _TMP_VEC;
    extern glm::vec3 _TMP_AXIS;
    extern glm::vec3 _BASE;
    extern float DOT_RADIUS;

    class Selection
    {
    private:
        Buffer* _circleBuffer;
        Buffer* _dotBuffer;
        glm::mat4 _cacheDotMVP;
        glm::mat4 _cacheDotSymMVP;
        glm::mat4 _cacheCircleMVP;
        glm::vec3 _color;
        float _offsetX;
        bool _isEditMode;
    public:
        Selection();


        inline Buffer* getCircleBuffer() {
            return _circleBuffer;
        }

        inline const Buffer* getCircleBuffer() const {
            return _circleBuffer;
        }

        inline Buffer* getDotBuffer() {
            return _dotBuffer;
        }

        inline const Buffer* getDotBuffer() const {
            return _dotBuffer;
        }

        inline glm::mat4& getCircleMVP() {
            return _cacheDotMVP;
        }

        inline glm::mat4& getDotMVP() {
            return _cacheDotMVP;
        }

        inline glm::mat4& getDotSymmetryMVP() {
            return _cacheDotSymMVP;
        }

        inline glm::vec3& getColor() {
            return _color;
        }

        inline const glm::mat4& getCircleMVP() const {
            return _cacheDotMVP;
        }

        inline const glm::mat4& getDotMVP() const {
            return _cacheDotMVP;
        }

        inline const glm::mat4& getDotSymmetryMVP() const {
            return _cacheDotSymMVP;
        }

        inline const glm::vec3& getColor() const {
            return _color;
        }

        inline void setIsEditMode(bool b) {
            _isEditMode = b;
        }

        inline bool getIsEditMode() const {
            return _isEditMode;
        }

        inline void setOffsetX(float offset) {
            _offsetX = offset;
        }

        inline float getOffsetX() const {
            return _offsetX;
        }

        void init();

        void release();

        Float32Array _getCircleVertices(float radius = 1.0, std::uint32_t nbVertices = 50, bool full = false);

        inline Float32Array _getDotVertices(float r, std::uint32_t nb)
        {
            return _getCircleVertices(r, nb, true);
        }

        void _updateMatricesBackground(Camera* camera, Scene* main);

        void _updateMatricesMesh(Camera* camera, Scene* main);

        void render(Scene* main);
    };
}

#endif // digital_sculpt_selection_h__