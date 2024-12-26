#ifndef terrain_h__
#define terrain_h__

#include "vertex.h"


struct TerrainCell
{
    glm::vec3 position;
    // 其他属性...
};

class Terrain
{
public:
    
public:
    Terrain(uint32_t zone_origin_x, uint32_t zone_origin_y, uint32_t zone_size);
    ~Terrain();
private:

    /**************************************************************************************
    根据数组索引值，算出对应的顶点世界坐标值
    @name: Terrain::MapPosition
    @return: glm::vec3
    @param: uint32_t x
    @param: uint32_t y
    *************************************************************************************/
    glm::vec3 MapPosition(uint32_t x, uint32_t y);

    /**************************************************************************************
    根据数组索引值，算出对应的顶点世界坐标值和纹理映射值
    @name: Terrain::CompileVertex
    @return: void
    @param: uint32_t x
    @param: uint32_t y
    *************************************************************************************/
    void CompileVertex(uint32_t x, uint32_t y);

    /**************************************************************************************

    @name: Terrain::CompileFan
    @return: void
    @param: uint32_t x1
    @param: uint32_t y1
    @param: uint32_t x2
    @param: uint32_t y2
    @param: uint32_t x3
    @param: uint32_t y3
    @param: uint32_t x4
    @param: uint32_t y4
    @param: uint32_t x5
    @param: uint32_t y5
    *************************************************************************************/
    void CompileFan(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t x3, uint32_t y3, uint32_t x4, uint32_t y4, uint32_t x5, uint32_t y5);


    /**************************************************************************************
    
    @name: Terrain::CompileFan
    @return: void
    @param: uint32_t x1
    @param: uint32_t y1
    @param: uint32_t x2
    @param: uint32_t y2
    @param: uint32_t x3
    @param: uint32_t y3
    @param: uint32_t x4
    @param: uint32_t y4
    @param: uint32_t x5
    @param: uint32_t y5
    @param: uint32_t x6
    @param: uint32_t y6
    *************************************************************************************/
    void CompileFan(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t x3, uint32_t y3, uint32_t x4, uint32_t y4, uint32_t x5, uint32_t y5, uint32_t x6, uint32_t y6);


    /**************************************************************************************

    @name: Terrain::CompileStrip
    @return: void
    @param: uint32_t x1
    @param: uint32_t y1
    @param: uint32_t x2
    @param: uint32_t y2
    @param: uint32_t x3
    @param: uint32_t y3
    @param: uint32_t x4
    @param: uint32_t y4
    *************************************************************************************/
    void CompileStrip(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t x3, uint32_t y3, uint32_t x4, uint32_t y4);

    /**************************************************************************************

                          North                 N
    *-------*           *---+---*           *---*---*     *---+---*
    |\      |           |\     /|           |\Nl|Nr/|     |   |   |
    | \ Sup |           | \   / |           | \ | / |     | A | B |
    |  \    |           |  \ /  |           |Wr\|/El|     |   |   |
    |   \   |       West+   *   +East      W*---*---*E    *---+---*
    |    \  |           |  / \  |           |Wl/|\Er|     |   |   |
    | Inf \ |           | /   \ |           | / | \ |     | C | D |
    |      \|           |/     \|           |/Sr|Sl\|     |   |   |
    *-------*           *---+---*           *---*---*     *---*---*
                          South                 S

    Figure a            Figure b            Figure c      Figure d

    This takes a single quadtree block and decides how to divide it for rendering.
    If the center point in not included in the mesh (or if there IS no center
    because we are at the lowest level of the tree), then the block is simply
    cut into two triangles. (Figure a)

    If the center point is active, but none of the edges, the block is cut into
    four triangles.  (Fig. b)  If the edges are active, then the block is cut
    into a combination of smaller triangles (Fig. c) and sub-blocks (Fig. d).

    @name: Terrain::CompileBlock
    @return: void
    @param: uint32_t x
    @param: uint32_t y
    @param: uint32_t size
    *************************************************************************************/
    void CompileBlock(uint32_t x, uint32_t y, uint32_t size);

    void Compile();

    void UploadData();

    inline bool POINT(uint32_t x, uint32_t y)
    {
        return m_point[x + y * m_map_size] == 0 ? false : true;
    }
private:
    GLuint VAO, VBO, EBO;

    uint32_t m_vertices;
    uint32_t m_triangles;

    // 假设有一个纹理坐标数组和地图数据
    //glm::vec2 m_zone_uv[(MAP_AREA + 1) * (MAP_AREA + 1)];
    std::vector<glm::vec2> m_zone_uv;
    TerrainCell map[MAP_AREA + 1][MAP_AREA + 1];

    // 存储顶点和索引数据
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    std::vector<uint8_t> m_point;

    uint32_t m_zone_origin_x;
    uint32_t m_zone_origin_y;
    uint32_t m_map_size;
    uint32_t m_map_half;
    uint32_t m_zone_size;
    uint32_t m_zone;
    bool m_use_color;
};

#endif // terrain_h__
