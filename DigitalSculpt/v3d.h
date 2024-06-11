#ifndef v3d_h__
#define v3d_h__

//定义一个基本3D顶点类，一个3D顶点至少有position，color，normal和第一层纹理uv这四个属性

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "glm/glm.hpp"
/*
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
*/

#define NOTUSINGNORMALORUV

namespace DigitalSculpt
{
    extern const float DefaultAlphaValue;
    extern const glm::vec4 DefaultColorLoader;
    extern const unsigned int DefaultBinaryWeightData;

    struct V3D
    {
    public:
        V3D();
        V3D(const glm::vec3& pos);
        V3D(float x, float y, float z);
        V3D(float x, float y, float z, float r, float g, float b);
        ~V3D();

        /******************************************************************************************************************
         * Desc:
         * Method:    operator=
         * Returns:   void
         * Parameter: const glm::vec3 & newPosition
         ****************************************************************************************************************/
        void operator=(const glm::vec3& newPosition);  // can be used to load a position directly into the vertex

        /******************************************************************************************************************
         * Desc:
         * Method:    operator=
         * Returns:   void
         * Parameter: const glm::vec4 & newColor
         ****************************************************************************************************************/
        void operator=(const glm::vec4& newColor);     // can be used to load a color directly into the vertex


        /******************************************************************************************************************
         * Desc:
         * Method:    compare
         * Returns:   bool
         * Parameter: const V3D & cmp
         ****************************************************************************************************************/
        bool compare(const V3D& cmp) const;

        /******************************************************************************************************************
         * Desc:
         * Method:    operator+=
         * Returns:   void
         * Parameter: const glm::vec3 & positionOffset
         ****************************************************************************************************************/
        void operator+=(const glm::vec3& positionOffset); // shift vertices
        /******************************************************************************************************************
         * Desc:
         * Method:    operator-=
         * Returns:   void
         * Parameter: const glm::vec3 & positionOffset
         ****************************************************************************************************************/
        void operator-=(const glm::vec3& positionOffset);

        /******************************************************************************************************************
         * Desc:
         * Method:    operator+=
         * Returns:   void
         * Parameter: const glm::vec4 & addColor
         ****************************************************************************************************************/
        void operator+=(const glm::vec4& addColor);       // shift colot
        /******************************************************************************************************************
         * Desc:
         * Method:    operator-=
         * Returns:   void
         * Parameter: const glm::vec4 & colorOffset
         ****************************************************************************************************************/
        void operator-=(const glm::vec4& colorOffset);

        /******************************************************************************************************************
         * Desc:
         * Method:    setPositionToCenter
         * Returns:   void
         ****************************************************************************************************************/
        void setPositionToCenter(); // forces the position to go to (0,0,0) - debugging only.

        /******************************************************************************************************************
         * Desc:
         * Method:    loadColorAs255
         * Returns:   void
         * Parameter: int r
         * Parameter: int g
         * Parameter: int b
         ****************************************************************************************************************/
        void loadColorAs255(int r, int g, int b); // load in a color by a 255 color scale(easier to understand wtf is happening)
        /******************************************************************************************************************
         * Desc:
         * Method:    loadColorDirectly
         * Returns:   void
         * Parameter: const glm::vec4 & colorVector
         ****************************************************************************************************************/
        void loadColorDirectly(const glm::vec4& colorVector);
        /******************************************************************************************************************
         * Desc:
         * Method:    printPosition
         * Returns:   void
         ****************************************************************************************************************/
        void printPosition(); // print position to console
        /******************************************************************************************************************
         * Desc:
         * Method:    printColor
         * Returns:   void
         ****************************************************************************************************************/
        void printColor();    // print color to console - excludes alpha which is assumed 1.0

        /******************************************************************************************************************
         * Desc:
         * Method:    printSlots
         * Returns:   void
         ****************************************************************************************************************/
        void printSlots();                      // print all slot data.
        /******************************************************************************************************************
         * Desc:
         * Method:    removeTriangle
         * Returns:   void
         * Parameter: int triangleIndex
         ****************************************************************************************************************/
        void removeTriangle(int triangleIndex); // erase a triangle from the vertex
        /******************************************************************************************************************
         * Desc:
         * Method:    addTriangle
         * Returns:   void
         * Parameter: int triangleIndex
         ****************************************************************************************************************/
        void addTriangle(int triangleIndex);    // add a triangle to the vertex

        glm::vec3 position;
        glm::vec4 color; // shrink to glm::vec3 eventually.
        glm::vec3 normal;


        std::unordered_set<int> triangleIDs; // Triangles that this vertex is a part of.  - 100% SUSSY 


#ifndef NOTUSINGNORMALORUV
        v2 uv;
#endif // !NOTUSINGNORMALORUV
    };



}

#endif // v3d_h__
//定义