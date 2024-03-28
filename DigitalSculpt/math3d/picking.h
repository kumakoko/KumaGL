#ifndef digital_sculpt_picking_h__
#define digital_sculpt_picking_h__

#include <cmath>
#include <string>
#include <map>
#include "glm/vec3.hpp"
#include "math3d/alpha_info.h"
#include "mesh/mesh.h"

namespace DigitalSculpt
{
    extern glm::vec3 _TMP_NEAR;
    extern glm::vec3 _TMP_NEAR_1;
    extern glm::vec3 _TMP_FAR;
    extern glm::mat4 _TMP_INV;
    extern glm::vec3 _TMP_INTER;
    extern glm::vec3 _TMP_INTER_1;
    extern glm::vec3 _TMP_V1;
    extern glm::vec3 _TMP_V2;
    extern glm::vec3 _TMP_V3;

    class Picking
    {
    public:
        static constexpr const char* INIT_ALPHAS_NAMES[] = { "alphaSquare", "alphaSkin" };
        static constexpr const char* INIT_ALPHAS_PATHS[] = { "asquare.jpg", "skin.jpg" };
        static std::map<std::string, AlphaInfo*> ALPHAS;
    public:
        static void readAlphas();
        static AlphaInfo* addAlpha(kgl::TextureSPtr u8, float width, float height, const std::string& name);
    private:
        Mesh* _mesh; // mesh
        Scene* _main; // the camera
        AlphaInfo* _alpha;// = null;
        Uint32Array _pickedVertices;// = []; // vertices selected
        std::int32_t _pickedFace;// = -1; // face picked
        bool _xSym;// = !!xSym;
        float _alphaSide;// = 0.0;
        float _rLocal2;// = 0.0; // radius of the selection area (local/object space)
        float _rWorld2;// = 0.0; // radius of the selection area (world space)
        glm::vec3 _pickedNormal;// = [0.0, 0.0, 0.0];
        glm::vec3 _alphaOrigin;// = [0.0, 0.0, 0.0]; // alpha stuffs
        glm::mat4 _alphaLookAt;// = mat4.create();
        glm::vec3 _interPoint;// = [0.0, 0.0, 0.0]; // intersection point (mesh local space)
        glm::vec3  _eyeDir;// = [0.0, 0.0, 0.0]; // eye direction
    public:
        inline void setIdAlpha(const std::string& id)
        {
            _alpha = Picking::ALPHAS[id];
        }

        inline void initAlpha()
        {
            computePickedNormal();
            updateAlpha(false);
        }

        inline Mesh* getMesh()
        {
            return _mesh;
        }

        inline void setLocalRadius2(float radius)
        {
            _rLocal2 = radius;
        }

        inline float getLocalRadius2()
        {
            return _rLocal2;
        }

        inline float getLocalRadius() const
        {
            return std::sqrt(_rLocal2);
        }

        inline float getWorldRadius2() const
        {
            return _rWorld2;
        }

        inline float getWorldRadius() const
        {
            return std::sqrt(_rWorld2);
        }

        inline void setIntersectionPoint(const glm::vec3& inter)
        {
            _interPoint = inter;
        }

        inline glm::vec3& getEyeDirection()
        {
            return _eyeDir;
        }

        inline const glm::vec3& getEyeDirection() const
        {
            return _eyeDir;
        }

        inline glm::vec3& getIntersectionPoint()
        {
            return _interPoint;
        }

        inline const glm::vec3& getIntersectionPoint() const
        {
            return _interPoint;
        }

        inline Uint32Array& getPickedVertices()
        {
            return _pickedVertices;
        }

        inline const Uint32Array& getPickedVertices() const
        {
            return _pickedVertices;
        }

        inline std::int32_t getPickedFace() const
        {
            return _pickedFace;
        }

        inline glm::vec3& getPickedNormal()
        {
            return _pickedNormal;
        }

        inline const glm::vec3& getPickedNormal() const
        {
            return _pickedNormal;
        }

        /***************************************************
        
        @name: DigitalSculpt::Picking::Picking
        @return: 
        @param: Scene * main
        @param: bool xSym
        ***************************************************/
        Picking(Scene* main, bool xSym);

        /***************************************************
        
        @name: DigitalSculpt::Picking::getAlpha
        @return: float
        @param: float x
        @param: float y
        @param: float z
        ***************************************************/
        float getAlpha(float x, float  y, float  z);

        /***************************************************
        
        @name: DigitalSculpt::Picking::updateAlpha
        @return: void
        @param: bool keepOrigin
        ***************************************************/
        void updateAlpha(bool keepOrigin);

        /******************************************************************************************************************
         * Desc:
         * Method:    intersectionMouseMesh
         * Returns:   bool
         * Parameter: Mesh * mesh
         * Parameter: float mouseX
         * Parameter: float mouseY
         ****************************************************************************************************************/
        bool intersectionMouseMesh(Mesh* mesh, float mouseX, float mouseY);

        /******************************************************************************************************************
         * Desc: Intersection between a ray the mouse position for every meshes
         * Method:    intersectionMouseMeshes
         * Returns:   void
         * Parameter: std::vector<Mesh * > & meshes
         * Parameter: float mouseX
         * Parameter: float mouseY
         ****************************************************************************************************************/
        bool intersectionMouseMeshes(std::vector<Mesh*>& meshes, float mouseX, float mouseY);

        /******************************************************************************************************************
         * Desc: Intersection between a ray and a mesh
         * Method:    intersectionRayMesh
         * Returns:   bool
         * Parameter: Mesh * mesh
         * Parameter: const glm::vec3 & vNearOrig
         * Parameter: const glm::vec3 & vFarOrig
         ****************************************************************************************************************/
        bool intersectionRayMesh(Mesh* mesh, const glm::vec3& vNearOrig, const glm::vec3& vFarOrig);

        /******************************************************************************************************************
         * Desc: Find all the vertices inside the sphere
         * Method:    pickVerticesInSphere
         * Returns:   DigitalSculpt::Uint32Array
         * Parameter: float rLocal2
         ****************************************************************************************************************/
        Uint32Array& pickVerticesInSphere(float rLocal2);

        /******************************************************************************************************************
         * Desc:
         * Method:    _isInsideSphere
         * Returns:   bool
         * Parameter: std::uint32_t id
         * Parameter: const glm::vec3 & inter
         * Parameter: float rLocal2
         ****************************************************************************************************************/
        bool _isInsideSphere(std::uint32_t id, const glm::vec3& inter, float rLocal2) const;

        /******************************************************************************************************************
         * Desc: Find all the vertices inside the sphere (with topological check)
         * Method:    pickVerticesInSphereTopological
         * Returns:   DigitalSculpt::Uint32Array&
         * Parameter: float rLocal2
         ****************************************************************************************************************/
        Uint32Array& pickVerticesInSphereTopological(float rLocal2);

        /******************************************************************************************************************
         * Desc:
         * Method:    computeWorldRadius2
         * Returns:   float
         * Parameter: bool ignorePressure
         ****************************************************************************************************************/
        float computeWorldRadius2(bool ignorePressure);

        /******************************************************************************************************************
         * Desc:
         * Method:    updateLocalAndWorldRadius2
         * Returns:   float
         ****************************************************************************************************************/
        float updateLocalAndWorldRadius2();

        /******************************************************************************************************************
         * Desc:
         * Method:    unproject
         * Returns:   glm::vec3
         * Parameter: float x
         * Parameter: float y
         * Parameter: float z
         ****************************************************************************************************************/
        glm::vec3 unproject(float x, float y, float z);

        /******************************************************************************************************************
         * Desc:
         * Method:    project
         * Returns:   void
         * Parameter: glm::vec3 vec
         ****************************************************************************************************************/
        void project(glm::vec3 vec);

        /******************************************************************************************************************
         * Desc:
         * Method:    computePickedNormal
         * Returns:   glm::vec3*
         ****************************************************************************************************************/
        glm::vec3* computePickedNormal();

        /******************************************************************************************************************
         * Desc:
         * Method:    polyLerp
         * Returns:   glm::vec3&
         * Parameter: const Float32Array & vField
         * Parameter: glm::vec3 & out
         ****************************************************************************************************************/
        glm::vec3& polyLerp(const Float32Array& vField, glm::vec3& out);
    };
}

#endif // digital_sculpt_picking_h__