/**************************************************************************************************************************
Copyright(C) 2014-2019 www.xionggf.com

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute,sublicense, and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM,OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************************************/
/*!
 * \file kgl_particle_system.h
 * \date 2019/03/11 14:19
 *
 * \author www.xionggf.com
 * Contact: sun_of_lover@sina.com
 *
 * \brief
 *
 * TODO: long description
 *
 * \note
 */
#ifndef kgl_particle_system_h__
#define kgl_particle_system_h__

#include "kgl_gpu_program.h"

namespace kgl
{
    /*
    class CParticle
    {
    public:
        glm::vec3 vPosition;
        glm::vec3 vVelocity;
        glm::vec3 vColor;
        float fLifeTime;
        float fSize;
        int iType;
    };

    class CParticleSystemTransformFeedback
    {
    public:
        bool InitalizeParticleSystem();

        void RenderParticles();
        void UpdateParticles(float fTimePassed);

        void SetGeneratorProperties(glm::vec3 a_vGenPosition, glm::vec3 a_vGenVelocityMin, glm::vec3 a_vGenVelocityMax, glm::vec3 a_vGenGravityVector, glm::vec3 a_vGenColor, float a_fGenLifeMin, float a_fGenLifeMax, float a_fGenSize, float fEvery, int a_iNumToGenerate);

        void ClearAllParticles();
        bool ReleaseParticleSystem();

        int GetNumParticles();

        void SetMatrices(glm::mat4* a_matProjection, glm::vec3 vEye, glm::vec3 vView, glm::vec3 vUpVector);

        CParticleSystemTransformFeedback();

    private:
        bool bInitialized;

        UINT uiTransformFeedbackBuffer;

        UINT uiParticleBuffer[2];
        UINT uiVAO[2];

        UINT uiQuery;
        UINT uiTexture;

        int iCurReadBuffer;
        int iNumParticles;

        glm::mat4 matProjection, matView;
        glm::vec3 vQuad1, vQuad2;

        float fElapsedTime;
        float fNextGenerationTime;

        glm::vec3 vGenPosition;
        glm::vec3 vGenVelocityMin, vGenVelocityRange;
        glm::vec3 vGenGravityVector;
        glm::vec3 vGenColor;

        float fGenLifeMin, fGenLifeRange;
        float fGenSize;

        int iNumToGenerate;

        CShader shVertexRender, shGeomRender, shFragRender;
        CShader shVertexUpdate, shGeomUpdate, shFragUpdate;
        CShaderProgram spRenderParticles;
        CShaderProgram spUpdateParticles;

        GPUProgram* render_program_ = nullptr;
        GPUProgram* update_pragram_ = nullptr;
    };
    */
}

#endif // kgl_particle_system_h__