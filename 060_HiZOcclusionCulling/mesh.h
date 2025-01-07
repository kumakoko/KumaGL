/**************************************************************************************************************************
Copyright(C) 2014-2025 www.xionggf.com

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

#ifndef mesh_h__
#define mesh_h__

struct Vertex
{
    Vertex() 
    {
    }
    
    Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& tex) :
        position(position), normal(normal), tex(tex)
    {
    }

    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex;
};

struct AABB
{
    glm::vec4 minpos;
    glm::vec4 maxpos;
};

class Mesh
{
public:
    std::vector<Vertex> vbo;
    std::vector<uint16_t> ibo;
    AABB aabb;
public:
    /*********************************************************
    
    @param  const AABB & aabb
    @return Mesh    
    *********************************************************/
    static Mesh CreateBoxMesh(const AABB& aabb);

    /*********************************************************
    
    @param  float radius
    @param  const glm::vec3 & center
    @param  uint32_t vertices_per_circumference
    @return Mesh    
    *********************************************************/
    static Mesh CreateSphereMesh(float radius, const glm::vec3& center, uint32_t vertices_per_circumference);
};

class GLDrawable
{
public:
    GLDrawable(const Mesh& mesh);
    GLDrawable();
    ~GLDrawable();

    const AABB& get_aabb() const;

    GLuint get_vertex_array() const;
    uint32_t get_num_elements() const;

private:
    GLuint vertex_array;
    GLuint vertex_buffer;
    GLuint index_buffer;

    uint32_t num_elements;
    AABB aabb;
};



#endif // mesh_h__