#include <utility>
#include <iterator>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "mesh.h"
#include "misc/array_1d.h"
#include "misc/array_2d.h"
#include "math3d/octree_cell.h"

namespace DigitalSculpt
{
    void Mesh::render(Scene* main) 
    {
        if (!isVisible())
            return;
        
        Shader[getShaderType()].getOrCreate().draw(this, main);
    }

    void Mesh::renderWireframe(Scene* main)
    {
        if (!isVisible()) 
            return;
        
        Shader[Enums.Shader.WIREFRAME].getOrCreate().draw(this, main);
    }

    void Mesh::renderFlatColor(Scene* main)
    {
        if (!isVisible())
            return;
        
        Shader[Enums.Shader.FLAT].getOrCreate().draw(this, main);
    }
}