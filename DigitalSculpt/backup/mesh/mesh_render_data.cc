#include <utility>
#include <iterator>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" // 引入变换函数
#include "glm/gtc/type_ptr.hpp" // 引入变换函数
#include "mesh.h"
#include "misc/array_1d.h"
#include "misc/array_2d.h"

namespace DigitalSculpt
{
    void Mesh::setShowWireframe(bool showWireframe)
    {
        _renderData->_showWireframe = RenderData::ONLY_DRAW_ARRAYS ? false : showWireframe;
        updateWireframeBuffer();
    }

    std::uint32_t Mesh::getCount() const 
    {
        if (getMode() == GL_TRIANGLES) 
            return getNbTriangles() * 3;
        
        if (getMode() == GL_LINES)
            return getNbVertices();
        
        return 0;
    }

    void Mesh::setShaderType(int shaderName)
    {
        bool has_uv = this->hasUV();

        if (shaderName == Enums::Shader::UV && !has_uv) 
        {
            if (_renderData->_shaderType != Enums::Shader::UV)
                return;

            shaderName = Enums::Shader::MATCAP;
        }

        _renderData->_shaderType = shaderName;
        
        if (has_uv) 
        {
            updateDuplicateGeometry();
            updateDrawArrays();
        }

        updateBuffers();
    }

    void Mesh::initRender()
    {
        setShaderType(_renderData->_shaderType);
        setShowWireframe(getShowWireframe());
    }
}