#ifndef digital_sculpt_shader_base_h__
#define digital_sculpt_shader_base_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_texture.h"

#include <vector>
#include <string>
#include <regex>
#include <unordered_map>
#include <sstream>

#include "misc/get_options_url.h" //import getOptionsURL from "misc/getOptionsURL";
#include "misc/utils.h"
#include "render/attribute.h"
#include "render/shaders/glsl/glsl_curvature.h"
#include "render/attribute.h"
#include "mesh/mesh.h"
#include "scene.h"
//
//import Attribute from "render/Attribute";
//import colorSpaceGLSL from "render/shaders/glsl/colorSpace.glsl";
//import curvatureGLSL from "render/shaders/glsl/curvature.glsl";

namespace DigitalSculpt
{
    class ShaderBase
    {
    protected:
        std::string vertexName = "VertexName";
        std::string fragmentName = "FragmentName";
        std::string vertex;
        std::string fragment;

        GLuint program_;
        GLuint texture0;
        GLuint _dummyTex;

        Attribute* attribute_vertex;
        Attribute* attribute_normal;
        Attribute* attribute_material;
        Attribute* attribute_color;

        bool showSymmetryLine;// = getOptionsURL().mirrorline;
        bool darkenUnselected;// = getOptionsURL().darkenunselected;
        //ShaderBase.uniformNames = {};
        std::vector<std::string> commonUniforms = {
            "uMV", "uMVP", "uN", "uEM", "uEN", "uFlat", "uPlaneO",
            "uPlaneN", "uSym", "uCurvature", "uAlpha", "uFov", "uDarken" };

        std::vector<std::string> vertUniforms = { "uniform mat4 uMV;",
                "uniform mat4 uMVP;",
                "uniform mat3 uN;",
                "uniform mat4 uEM;",
                "uniform mat3 uEN;",
                "uniform float uAlpha;" };

        std::vector<std::string> fragColorUniforms = {
            "uniform vec3 uPlaneN;",
                "uniform vec3 uPlaneO;",
                "uniform int uSym;",
                "uniform int uDarken;",
                "uniform float uCurvature;",
                "uniform float uFov;",
                "varying float vMasking;",
                "uniform int uFlat;"
        };

        std::vector<std::string> fragColorFunction = {
            curvatureGLSL,
                colorSpaceGLSL,
                "#extension GL_OES_standard_derivatives : enable",
                "vec3 getNormal() {",
                "  #ifndef GL_OES_standard_derivatives",
                "    return normalize(gl_FrontFacing ? vNormal : -vNormal);",
                "  #else",
                "    return uFlat == 0 ? normalize(gl_FrontFacing ? vNormal : -vNormal) : -normalize(cross(dFdy(vVertex), dFdx(vVertex)));",
                "  #endif",
                "}",
                "vec4 encodeFragColor(const in vec3 frag, const in float alpha) {",
                "  vec3 col = computeCurvature(vVertex, vNormal, frag, uCurvature, uFov);",
                "  if(uDarken == 1) col *= 0.3;",
                "  col *= (0.3 + 0.7 * vMasking);",
                "  if(uSym == 1 && abs(dot(uPlaneN, vVertex - uPlaneO)) < 0.15)",
                "      col = min(col * 1.5, 1.0);",
                "  return alpha != 1.0 ? vec4(col * alpha, alpha) : encodeRGBM(col);",
                "}"
        };

        std::vector<std::string> uniformNames; // Names of the shader uniforms
        std::unordered_map<std::string, GLint> uniforms; // Map of uniform names to locations

    public:
        /******************************************************************************************************************
         * Desc:
         * Method:    processShader
         * Returns:   std::string
         * Parameter: const std::string & inputStr
         ****************************************************************************************************************/
        std::string processShader(const std::string& inputStr);

        /******************************************************************************************************************
         * Desc: 
         * Method:    getOrCreate
         * Returns:   DigitalSculpt::ShaderBase*
         ****************************************************************************************************************/
         ShaderBase* getOrCreate();

        /******************************************************************************************************************
         * Desc:
         * Method:    initUniforms
         * Returns:   void
         ****************************************************************************************************************/
        void initUniforms();

        // Assuming Main and Mesh classes are defined elsewhere with the required methods
        /******************************************************************************************************************
         * Desc:
         * Method:    updateUniforms
         * Returns:   void
         * Parameter: Mesh * mesh
         * Parameter: Scene * main
         ****************************************************************************************************************/
        void updateUniforms(Mesh* mesh, Scene* main);

        /******************************************************************************************************************
         * Desc:
         * Method:    draw
         * Returns:   void
         * Parameter: Mesh * mesh
         * Parameter: Scene * main
         ****************************************************************************************************************/
        void draw(Mesh* mesh, Scene* main);

        /******************************************************************************************************************
         * Desc: 
         * Method:    drawBuffer
         * Returns:   void
         * Parameter: Mesh * mesh
         ****************************************************************************************************************/
         void drawBuffer(Mesh* mesh);

        /******************************************************************************************************************
         * Desc: 
         * Method:    setTextureParameters
         * Returns:   void
         * Parameter: kgl::TextureSPtr tex
         ****************************************************************************************************************/
         void setTextureParameters(kgl::TextureSPtr tex);

        /******************************************************************************************************************
         * Desc: 
         * Method:    onLoadTexture0
         * Returns:   void
         * Parameter: GLuint tex
         * Parameter: Scene * main
         ****************************************************************************************************************/
         void onLoadTexture0(GLuint tex, Scene* main);

         /******************************************************************************************************************
          * Desc: 
          * Method:    getDummyTexture
          * Returns:   GLuint
          ****************************************************************************************************************/
          GLuint  getDummyTexture();

        /******************************************************************************************************************
         * Desc: 
         * Method:    getOrCreateTexture0
         * Returns:   bool
         * Parameter: std::string texPath
         * Parameter: Scene * main
         ****************************************************************************************************************/
         bool getOrCreateTexture0(std::string texPath, Scene* main);

        /******************************************************************************************************************
         * Desc: 
         * Method:    initAttributes
         * Returns:   void
         ****************************************************************************************************************/
         void initAttributes();

        /******************************************************************************************************************
         * Desc: 
         * Method:    bindAttributes
         * Returns:   void
         * Parameter: Mesh * mesh
         ****************************************************************************************************************/
         void bindAttributes(Mesh* mesh);

        /******************************************************************************************************************
         * Desc: 
         * Method:    unbindAttributes
         * Returns:   void
         ****************************************************************************************************************/
         void unbindAttributes();
    };
}

#endif // digital_sculpt_shader_base_h__