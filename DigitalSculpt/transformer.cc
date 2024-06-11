

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

#include "transformer.h"
#include "math_definitions.h"

namespace DigitalSculpt
{
    ModelMatrix::ModelMatrix()
    {
        model = glm::mat4(1);
        position = glm::vec3(0);
        translationValues = glm::vec3(0);
        rotationMatrix = glm::mat4(1);
        scaleValues = glm::vec3(1);
    }

    ModelMatrix::~ModelMatrix()
    {
    }

    void ModelMatrix::resetModelMatrix()
    {
        this->model = glm::mat4(1.0f);
    }

    void ModelMatrix::setModelMatrix()
    {
        resetModelMatrix();
        //translate model matrix to world origin
        translateAll(-1.0f * transform.worldTranslation);

        model = glm::scale(transform.scale) * transform.rotationMatrix * model;

        //translate matrix back then add new translation
        translateAll(transform.worldTranslation + transform.translation);

        position = transform.worldTranslation + transform.translation;
    }

    void ModelMatrix::rotateX(const float x)
    {
        glm::mat4 rotationMatrix = glm::rotate(x, Basis::X);
        model = rotationMatrix * model;
    }

    void ModelMatrix::rotateY(const float y)
    {
        glm::mat4 rotationMatrix = glm::rotate(y, Basis::Y);
        model = rotationMatrix * model;
    }

    void ModelMatrix::rotateZ(const float z)
    {
        glm::mat4 rotationMatrix = glm::rotate(z, Basis::Z);
        model = rotationMatrix * model;
    }

    void ModelMatrix::rotateX_deg(const float x)
    {
        glm::mat4 rotationMatrix = glm::rotate(glm::radians(x), Basis::X);
        model = rotationMatrix * model;
    }

    void ModelMatrix::rotateY_deg(const float y)
    {
        glm::mat4 rotationMatrix = glm::rotate(glm::radians(y), Basis::Y);
        model = rotationMatrix * model;
    }

    void ModelMatrix::rotateZ_deg(const float z)
    {
        glm::mat4 rotationMatrix = glm::rotate(glm::radians(z), Basis::Z);
        model = rotationMatrix * model;
    }

    void ModelMatrix::setRotation(const glm::mat4& r)
    {
        model = rotationMatrix * model;
    }

    void ModelMatrix::translateAll(const glm::vec3& t)
    {
        model[3][0] += t.x;
        model[3][1] += t.y;
        model[3][2] += t.z;
    }

    void ModelMatrix::translateX(const float x)
    {
        glm::mat4 translationMatrix = glm::translate(glm::vec3(x, 0, 0));
        model = translationMatrix * model;
    }

    void ModelMatrix::translateY(const float y)
    {
        glm::mat4 translationMatrix = glm::translate(glm::vec3(0, y, 0));
        model = translationMatrix * model;
    }

    void ModelMatrix::translateZ(const float z)
    {
        glm::mat4 translationMatrix = glm::translate(glm::vec3(0, 0, z));
        model = translationMatrix * model;
    }

    void ModelMatrix::setTranslation(const glm::vec3& t)
    {
        glm::mat4 translationMatrix = glm::translate(t);
        model = translationMatrix * model;
    }

    void ModelMatrix::scaleUniform(const float scalar)
    {
        glm::mat4 scaleMatrix = glm::scale(glm::vec3(scalar));
        model = scaleMatrix * model;
    }

    void ModelMatrix::scaleX(const float scalarx)
    {
        glm::mat4 scaleMatrix = glm::scale(glm::vec3(scalarx, 1.0f, 1.0f));
        model = scaleMatrix * model;
    }

    void ModelMatrix::scaleY(const float scalary)
    {
        glm::mat4 scaleMatrix = glm::scale(glm::vec3(1.0f, scalary, 1.0f));
        model = scaleMatrix * model;
    }

    void ModelMatrix::scaleZ(const float scalarz)
    {
        glm::mat4 scaleMatrix = glm::scale(glm::vec3(1.0f, 1.0f, scalarz));
        model = scaleMatrix * model;
    }

    void ModelMatrix::setScale(const glm::vec3& s)
    {
        glm::mat4 scaleMatrix = glm::scale(s);
        model = scaleMatrix * model;
    }

    void ModelMatrix::scaleWithRadius(const float radius)
    {
        auto mat = glm::scale(model, glm::vec3(radius));
        model = model * mat;
    }

    CameraTransformer::CameraTransformer()
    {
        view = glm::mat4(1.0f);
        projection = glm::mat4(1.0f);
        pos = glm::vec3(0.0f);
        startPos = glm::vec3(0.0f);
        focalPoint = glm::vec3(0.0f);
        orientation = glm::vec3(0.0f);
        up = glm::vec3(0.0f);
    }

    CameraTransformer::~CameraTransformer()
    {
    }
}
