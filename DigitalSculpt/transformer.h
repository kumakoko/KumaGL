#ifndef transformer_h__
#define transformer_h__

namespace DigitalSculpt
{
    class ModelMatrix
    {
    public:
        struct Transform
        {
            glm::mat4 rotationMatrix = glm::mat4(1);
            glm::vec3 translation = glm::vec3(0);
            glm::vec3 scale = glm::vec3(1);
            glm::vec3 worldTranslation = glm::vec3(0);
        };

        ModelMatrix();
        ~ModelMatrix();

        void resetModelMatrix();
        void setModelMatrix();

        // rotate the model x rad
        void rotateX(const float x);
        // rotate the model y rad
        void rotateY(const float y);
        // rotate the model z rad
        void rotateZ(const float z);

        // rotate the model x deg
        void rotateX_deg(const float x);
        // rotate the model y deg
        void rotateY_deg(const float y);
        // rotate the model z deg
        void rotateZ_deg(const float z);
        // set rotation in all axes
        void setRotation(const glm::mat4 t);

        // translate the model by a vector
        void translateAll(const glm::vec3& t);
        // translate the model x units in x
        void translateX(const float x);
        // translate the model y units in y
        void translateY(const float y);
        // translate the model z units in z
        void translateZ(const float z);
        // set translation in all axes
        void setTranslation(const glm::vec3& t);



        // scale the model uniformly
        void scaleUniform(const float scalar);

        // these are only here for *completeness*, you should not use them on any *deforming meshes*

        // scale the model by a scalar factor of x
        void scaleX(const float scalarx);
        // scale the model by a scalar factor of y
        void scaleY(const float scalary);
        // scale the model by a scalar factor of z
        void scaleZ(const float scalarz);
        // set scale in all axes
        void setScale(const glm::vec3& t);

        void scaleWithRadius(const float radius); // resets the model matrix then applies scale.

        glm::mat4 model = glm::mat4(1);

        Transform transform;
        glm::vec3 position;

        glm::vec3 translationValues = glm::vec3(0);
        glm::mat4 rotationMatrix = glm::mat4(1);
        glm::vec3 scaleValues = glm::vec3(1);
    };

    class CameraTransformer
    {
    public:
        CameraTransformer();
        ~CameraTransformer();

        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        glm::vec3 pos;
        glm::vec3 startPos;
        glm::vec3 focalPoint;
        glm::vec3 orientation;
        glm::vec3 up;
    };
}

#endif // transformer_h__
