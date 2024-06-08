#include "v3d.h"

//定义一个基本3D顶点类，一个3D顶点至少有position，color，normal和第一层纹理uv这四个属性

namespace DigitalSculpt
{
    const float DefaultAlphaValue = 1.0f;
    const glm::vec4 DefaultColorLoader = glm::vec4(0.7, 0.7, 0.7, DefaultAlphaValue); // start off with color of grey
    const unsigned int DefaultBinaryWeightData = 10000;// start off with a weight of 100.00 *technically*

    V3D::V3D()
    {
        position = glm::vec3(0);
        color = DefaultColorLoader;
        normal = glm::vec3(0);
    }

    V3D::V3D(const glm::vec3& pos)
    {
        position = pos;
        color = DefaultColorLoader;
        normal = glm::vec3(0);
    }

    V3D::V3D(float x, float y, float z)
    {
        position = glm::vec3(x, y, z);
        color = DefaultColorLoader;
        normal = glm::vec3(0);
    }

    V3D::V3D(float x, float y, float z, float r, float g, float b)
    {
        position = glm::vec3(x, y, z);
        color = glm::vec4(r, g, b, DefaultAlphaValue);
        normal = glm::vec3(0);
    }

    V3D::~V3D()
    {
    }

    void V3D::operator=(const glm::vec3& newPosition)
    {
        position = newPosition;
    }

    void V3D::operator=(const glm::vec4& newColor)
    {
        color = newColor;
    }

    bool V3D::compare(const V3D& cmp) const
    {
        return position == cmp.position && color == cmp.color;
    }

    void V3D::operator+=(const glm::vec3& positionOffset)
    {
        position += positionOffset;
    }

    void V3D::operator-=(const glm::vec3& positionOffset)
    {
        position -= positionOffset;
    }

    void V3D::operator+=(const glm::vec4& colorOffset)
    {
        color.r = (color.r + colorOffset.r > 1.0f) ? 1.0f : color.r + colorOffset.r;
        color.g = (color.g + colorOffset.g > 1.0f) ? 1.0f : color.g + colorOffset.g;
        color.b = (color.b + colorOffset.b > 1.0f) ? 1.0f : color.b + colorOffset.b;
    }

    void V3D::operator-=(const glm::vec4& colorOffset)
    {
        color.r = (color.r - colorOffset.r < 0.0f) ? 0.0f : color.r - colorOffset.r;
        color.g = (color.g - colorOffset.g > 0.0f) ? 0.0f : color.g - colorOffset.g;
        color.b = (color.b - colorOffset.b > 0.0f) ? 0.0f : color.b - colorOffset.b;
    }

    void V3D::setPositionToCenter()
    {
        position = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    void V3D::loadColorAs255(int r, int g, int b)
    {
        color = glm::vec4((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, 1.0f);
    }

    void V3D::loadColorDirectly(const glm::vec4& colorVector)
    {
        this->color = colorVector;
    }

    void V3D::printPosition()
    {
        //say "pos(" << position.x spc position.y spc position.z << ")" done;
    }

    // assume alpha is 1 always
    void V3D::printColor()
    {
        //say "col(" spc color.r spc color.g spc color.b << ")" done;
    }

    void V3D::printSlots()
    {
        printPosition();
        printColor();
    }

    void V3D::removeTriangle(int triangleIndex)
    {
        triangleIDs.erase(triangleIndex);
    }

    void V3D::addTriangle(int triangleIndex)
    {
        triangleIDs.insert(triangleIndex);
    }
}