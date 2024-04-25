#include "../klib/kgl_error.h"
#include "../klib/kgl_string_convertor.h"
#include "render/shaders/shader_base.h"

namespace DigitalSculpt
{
    ShaderBase::~ShaderBase()
    {
    }

    ShaderBase* ShaderBase::getOrCreate()
    {
        if (this->program_)
            return this;

        auto vname = (boost::format("\n#define SHADER_NAME %1% \n") % this->vertexName).str();
        auto fname = (boost::format("\n#define SHADER_NAME %1% \n") % this->fragmentName).str();

        GLint success = 0;
        GLchar info_log[512];
        const char* shader_src_code_pointer = nullptr;

        GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
        shader_src_code_pointer = processShader(this->vertex.append(vname)).c_str();
        glShaderSource(vShader, 1, &shader_src_code_pointer, nullptr);
        glCompileShader(vShader);
        glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(vShader, 512, nullptr, info_log);
            throw kgl::Error(kgl::StringConvertor::ANSItoUTF16LE(info_log), __FILE__, __LINE__);
        }

        GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
        shader_src_code_pointer = processShader(this->vertex.append(fname)).c_str();
        glShaderSource(fShader, 1, &shader_src_code_pointer, nullptr);
        glCompileShader(fShader);
        glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(vShader, 512, nullptr, info_log);
            throw kgl::Error(kgl::StringConvertor::ANSItoUTF16LE(info_log), __FILE__, __LINE__);
        }

        this->program_ = glCreateProgram();

        glAttachShader(program_, vShader);
        glAttachShader(program_, fShader);
        glLinkProgram(program_);
        glGetProgramiv(this->program_, GL_LINK_STATUS, &success);

        if (!success)
        {
            glGetProgramInfoLog(this->program_, 512, nullptr, info_log);
            throw kgl::Error(kgl::StringConvertor::ANSItoUTF16LE(info_log), __FILE__, __LINE__);
        }

        glUseProgram(program_);

        this->initAttributes();
        this->initUniforms();

        // no clean up for quick webgl inspector debugging
        glDetachShader(program_, fShader);
        glDeleteShader(fShader);
        glDetachShader(program_, vShader);
        glDeleteShader(vShader);
        return this;
    };

    std::string  ShaderBase::processShader(const std::string& inputStr)
    {
        // Move extension enable/require to the top of the file
        std::string extensions;
        std::string str = inputStr;
        std::regex regExtension("^\\s*(#extension).*", std::regex_constants::multiline);
        std::smatch matches;
        std::string::const_iterator searchStart(str.cbegin());
        std::unordered_map<std::string, bool> extMap;

        while (std::regex_search(searchStart, str.cend(), matches, regExtension)) {
            std::string ext = matches[0].str();
            searchStart = matches.suffix().first;
            str.erase(str.find(ext), ext.length());

            if (extMap.find(ext) == extMap.end()) {
                extMap[ext] = true;
                extensions += ext + "\n";
            }
        }

        std::string version;

        if (str.find("#version") == std::string::npos)
        {
            version += "#version 100\n";
        }

        std::string precision;
        std::regex regPrecision("precision\\s+(high|low|medium)p\\s+float");
        if (!std::regex_search(str, regPrecision)) {
            precision += "#ifdef GL_FRAGMENT_PRECISION_HIGH\n  precision highp float;\n#else\n  precision mediump float;\n#endif\n";
        }

        str = version + extensions + precision + str;
        return str;
    }

    void ShaderBase::initUniforms()
    {
        // Iterate over all uniform names and retrieve their locations
        for (size_t i = 0; i < uniformNames.size(); ++i)
        {
            const std::string& name = uniformNames[i];
            uniforms[name] = glGetUniformLocation(program_, name.c_str());
        }
    }


    // Assuming Main and Mesh classes are defined elsewhere with the required methods
    void ShaderBase::updateUniforms(Mesh* mesh, Scene* main)
    {
        static glm::vec3 tmp(0.0f, 0.0f, 0.0f); // Static to mimic the closure in the JavaScript version

        bool darken = darkenUnselected && main->getIndexSelectMesh(mesh) < 0;
        bool useSym = showSymmetryLine && (mesh->getID() == main->getMesh()->getID()) &&
            main->getSculptManager()->getSymmetry() != nullptr;

        glUniformMatrix4fv(uniforms["uEM"], 1, GL_FALSE, glm::value_ptr(mesh->getEditMatrix()));
        glUniformMatrix3fv(uniforms["uEN"], 1, GL_FALSE, glm::value_ptr(mesh->getEN()));
        glUniformMatrix4fv(uniforms["uMV"], 1, GL_FALSE, glm::value_ptr(mesh->getMV()));
        glUniformMatrix4fv(uniforms["uMVP"], 1, GL_FALSE, glm::value_ptr(mesh->getMVP()));
        glUniformMatrix3fv(uniforms["uN"], 1, GL_FALSE, glm::value_ptr(mesh->getN()));

        glUniform1i(uniforms["uDarken"], darken ? 1 : 0);
        glUniform1i(uniforms["uFlat"], mesh->getFlatShading());
        // Assuming vec3 and transformMat4 are methods from a library like glm
        glUniform3fv(uniforms["uPlaneO"], 1, glm::value_ptr(glm::transform(tmp, mesh->getSymmetryOrigin(), mesh->getMV())));
        glUniform3fv(uniforms["uPlaneN"], 1, glm::value_ptr(glm::normalize(glm::transformMat3(tmp, mesh->getSymmetryNormal(), mesh->getN()))));
        glUniform1i(uniforms["uSym"], useSym ? 1 : 0);
        glUniform1f(uniforms["uAlpha"], mesh->getOpacity());

        glUniform1f(uniforms["uCurvature"], mesh->getCurvature());
        auto cam = main->getCamera();
        glUniform1f(uniforms["uFov"], cam->isOrthographic() ? -std::fabs(cam->getTranslation()[2]) * 25.0f : cam->getFov());
    }


    void ShaderBase::draw(Mesh* mesh, Scene* main)
    {
        //var gl = mesh.getGL();
        glUseProgram(program_);
        this->bindAttributes(mesh);
        this->updateUniforms(mesh, main);
        this->drawBuffer(mesh);
    }

    void ShaderBase::drawBuffer(Mesh* mesh)
    {
        if (mesh->isUsingDrawArrays()) 
        {
            glDrawArrays(mesh->getMode(), 0, mesh->getCount());
        }
        else
        {
            mesh->getIndexBuffer()->bind();
            glDrawElements(mesh->getMode(), mesh->getCount(), GL_UNSIGNED_INT, 0);
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        this->unbindAttributes();
    }

    void ShaderBase::setTextureParameters(kgl::TextureSPtr tex)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (Utils::isPowerOfTwo(tex->GetWidth()) && Utils::isPowerOfTwo(tex->GetHeight()))
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
    }

    void ShaderBase::onLoadTexture0(GLuint tex, Scene* main)
    {
        glGenTextures(1, &texture0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, tex);
        setTextureParameters(tex);
        glBindTexture(GL_TEXTURE_2D, 0);
        if (main != nullptr)
            main->render();
    }

    GLuint  ShaderBase::getDummyTexture()
    {
        if (this->_dummyTex != 0)
            return this->_dummyTex;
        glGenTextures(1, &this->_dummyTex);//glCreateTexture();
        glBindTexture(GL_TEXTURE_2D, this->_dummyTex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, new Uint8Array(4));
        glBindTexture(GL_TEXTURE_2D, null);
        return this->_dummyTex;
    }

    bool ShaderBase::getOrCreateTexture0(std::string texPath, Scene* main)
    {
        if (this->texture0 != = undefined)
            return this->texture0;
        this->texture0 = null; // trigger loading
        var tex = new Image();
        tex.src = texPath;
        tex.onload = this->onLoadTexture0.bind(this, tex, main);
        return false;
    };

    void ShaderBase::initAttributes()
    {
        //var program = this->program_;
        //var attrs = this->attributes;
        attribute_vertex = new Attribute(program_, "aVertex", 3, GL_FLOAT);
        attribute_normal = new Attribute(program_, "aNormal", 3, GL_FLOAT);
        attribute_color = new Attribute(program_, "aColor", 3, GL_FLOAT);
        attribute_material = new Attribute(program_, "aMaterial", 3, GL_FLOAT);
    };

    void ShaderBase::bindAttributes(Mesh* mesh)
    {
        if (attribute_vertex != nullptr) 
            attribute_vertex->bindToBuffer(mesh->getVertexBuffer());
        if (attribute_normal != nullptr) 
            attribute_normal->bindToBuffer(mesh->getNormalBuffer());
        if (attribute_color != nullptr)
            attribute_color->bindToBuffer(mesh->getColorBuffer());
        if (attribute_material != nullptr) 
            attribute_material->bindToBuffer(mesh->getMaterialBuffer());
    };

    void ShaderBase::unbindAttributes()
    {
        if (attribute_vertex != nullptr)
            attribute_vertex->unbind();
        if (attribute_normal != nullptr)
            attribute_normal->unbind();
        if (attribute_color != nullptr)
            attribute_color->unbind();
        if (attribute_material != nullptr)
            attribute_material->unbind();
    }
}