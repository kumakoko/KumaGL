#include "indexed_triangle.h"

namespace DigitalSculpt
{
    IndexedTriangle::IndexedTriangle()
    {
        indice[0] = 0;
        indice[1] = 0;
        indice[2] = 0;
    }

    IndexedTriangle::IndexedTriangle(GLuint v0, GLuint v1, GLuint v2)
    {
        indice[0] = v0;
        indice[1] = v1;
        indice[2] = v2;
    }

    IndexedTriangle::~IndexedTriangle()
    {
    }

    GLuint& IndexedTriangle::operator[](const int pos)
    {
        return indice[pos];
    }

    const GLuint& IndexedTriangle::operator[](const int pos) const
    {
        return indice[pos];
    }

    void IndexedTriangle::print()
    {
        //say indice[0] spc indice[1] spc indice[2] << ' ';
    }

    void IndexedTriangle::delevel()
    {
        indice[0] = indice[0] - 1;
        indice[1] = indice[1] - 1;
        indice[2] = indice[2] - 1;
    }

    void IndexedTriangle::swapVertexIndex(int oldIndex, int newIndex)
    {
        if (indice[0] == oldIndex)
        {
            indice[0] = newIndex;
        }
        else if (indice[1] == oldIndex)
        {
            indice[1] = newIndex;
        }
        else if (indice[2] == oldIndex)
        {
            indice[2] = newIndex;
        }
        else
        {
            /*say "Error:\tCannot swap vertex '" << oldIndex << "' for '" << newIndex << "'"
                << "\n\tValid indices:" spc indice[0] spc indice[1] spc indice[2] done;*/
        }
    }
}