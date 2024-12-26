#include "../klib/kgl_lib_pch.h"

Terrain::Terrain(uint32_t size) :m_vertices(0), m_triangles(0)
{
    m_map_size = size;
    m_map_half = size / 2;
    m_zone_size = m_map_size / ZONE_GRID;

    m_point.resize(size * size, 0);

    m_zone_uv.resize((m_zone_size + 1) * (m_zone_size + 1)); //= new GLvector2[(m_zone_size + 1) * (m_zone_size + 1)];

    for (uint32_t x = 0; x <= m_zone_size; x++)
    {
        for (uint32_t y = 0; y <= m_zone_size; y++)
        {
            float u = static_cast<float>(x) / static_cast<float>(m_zone_size);
            float v = static_cast<float>(y) / static_cast<float>(m_zone_size);
            m_zone_uv[x + y * (m_zone_size + 1)] = glm::vec2(u, v);
        }
    }

    // 初始化VAO, VBO, EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * MAX_VERTICES, nullptr, GL_DYNAMIC_DRAW);

    // Element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned uint32_t) * MAX_INDICES, nullptr, GL_DYNAMIC_DRAW);

    // Vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, TexCoord)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Terrain::~Terrain()
{
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

// 定义MapPosition函数
glm::vec3 Terrain::MapPosition(uint32_t x, uint32_t y)
{
    x = std::clamp(x, 0, MAP_AREA);
    y = std::clamp(y, 0, MAP_AREA);
    return map[x][y].position;
}

// 创建顶点
void Terrain::CompileVertex(uint32_t x, uint32_t y)
{
    // 获取纹理坐标
    glm::vec2 uv = m_zone_uv[(x - m_zone_origin_x) + (y - m_zone_origin_y) * (m_zone_size + 1)];

    // 获取顶点位置
    glm::vec3 p = MapPosition(x, y);

    // 创建顶点并添加到列表
    Vertex vertex;
    vertex.Position = p;
    vertex.TexCoord = uv;
    vertices.push_back(vertex);

    // 添加索引
    indices.push_back(m_vertices++);
}

// 定义CompileFan函数
void Terrain::CompileFan(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2,
    uint32_t x3, uint32_t y3, uint32_t x4, uint32_t y4, uint32_t x5, uint32_t y5)
{
    // 开始一个新的扇形
    CompileVertex(x1, y1); // 顶点1
    CompileVertex(x5, y5); // 中心顶点
    CompileVertex(x4, y4); // 顶点4
    CompileVertex(x3, y3); // 顶点3
    CompileVertex(x2, y2); // 顶点2

    // 添加索引以形成扇形
    // 假设第一个顶点为中心
    uint32_t centerIndex = m_vertices - 4;

    for (uint32_t i = 1; i < 4; ++i)
    {
        indices.push_back(centerIndex);
        indices.push_back(centerIndex + i);
        indices.push_back(centerIndex + i + 1);
    }

    m_triangles += 3;
}


void Terrain::CompileFan(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t x3, uint32_t y3,
    uint32_t x4, uint32_t y4, uint32_t x5, uint32_t y5, uint32_t x6, uint32_t y6)
{
    // 添加中心顶点
    unsigned uint32_t center = CompileVertex(x1, y1);

    // 添加外围顶点
    uint32_t v1 = CompileVertex(x6, y6);
    uint32_t v2 = CompileVertex(x5, y5);
    uint32_t v3 = CompileVertex(x4, y4);
    uint32_t v4 = CompileVertex(x3, y3);
    uint32_t v5 = CompileVertex(x2, y2);

    // 创建四个三角形
    // Triangle 1: (center, v1, v2)
    indices.push_back(center);
    indices.push_back(v1);
    indices.push_back(v2);
    m_triangles++;

    // Triangle 2: (center, v2, v3)
    indices.push_back(center);
    indices.push_back(v2);
    indices.push_back(v3);
    m_triangles++;

    // Triangle 3: (center, v3, v4)
    indices.push_back(center);
    indices.push_back(v3);
    indices.push_back(v4);
    m_triangles++;

    // Triangle 4: (center, v4, v5)
    indices.push_back(center);
    indices.push_back(v4);
    indices.push_back(v5);
    m_triangles++;
}

// CompileStrip 函数（转写后的 OpenGL Core 3.3 版本）
void Terrain::CompileStrip(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t x3, uint32_t y3, uint32_t x4, uint32_t y4)
{
    // Add vertices in strip order
    CompileVertex(x1, y1);
    CompileVertex(x2, y2);
    CompileVertex(x3, y3);
    CompileVertex(x4, y4);

    // Convert triangle strip to triangles
    // Triangle 1: v0, v1, v2
    // Triangle 2: v2, v1, v3

    uint32_t v0 = m_vertices - 4;
    uint32_t v1 = m_vertices - 3;
    uint32_t v2 = m_vertices - 2;
    uint32_t v3 = m_vertices - 1;

    indices.push_back(v0);
    indices.push_back(v1);
    indices.push_back(v2);

    indices.push_back(v2);
    indices.push_back(v1);
    indices.push_back(v3);

    m_triangles += 2;
}

// CompileBlock 函数（转写后的 OpenGL Core 3.3 版本）
void Terrain::CompileBlock(uint32_t x, uint32_t y, uint32_t size)
{
    uint32_t next_size = size / 2;
    uint32_t x2 = x + size;
    uint32_t y2 = y + size;
    uint32_t xc = x + next_size;
    uint32_t yc = y + next_size;

    // 如果这是最小的块，或者中心点不活跃，则仅切分为两个三角形
    if (size == 1 || !POINT(xc, yc))
    {
        CompileStrip(x, y, x, y2, x2, y, x2, y2);
        return;
    }

    // 如果所有边缘点不活跃，则需要四个三角形
    if (!POINT(xc, y) && !POINT(xc, y2) && !POINT(x, yc) && !POINT(x2, yc))
    {
        CompileFan(xc, yc, x, y, x2, y, x2, y2, x, y2, x, y);
        return;
    }

    // 如果顶部和底部边缘点不活跃，则无法分割子块，创建一个单独的扇形
    if (!POINT(xc, y) && !POINT(xc, y2))
    {
        CompileVertex(xc, yc);
        CompileVertex(x, y);

        if (POINT(x, yc))
        {
            CompileVertex(x, yc); // W
            m_triangles++;
        }

        CompileVertex(x, y2);
        CompileVertex(x2, y2);

        if (POINT(x2, yc))
        {
            CompileVertex(x2, yc); // E
            m_triangles++;
        }

        CompileVertex(x2, y);
        CompileVertex(x, y);
        m_triangles += 4;
        return;
    }

    // 如果左侧和右侧边缘点不活跃，则无法分割子块，创建一个单独的扇形
    if (!POINT(x, yc) && !POINT(x2, yc))
    {
        CompileVertex(xc, yc);
        CompileVertex(x, y);
        CompileVertex(x, y2);

        if (POINT(xc, y2))
        {
            CompileVertex(xc, y2); // S
            m_triangles++;
        }

        CompileVertex(x2, y2);
        CompileVertex(x2, y);

        if (POINT(xc, y))
        {
            CompileVertex(xc, y); // N
            m_triangles++;
        }

        CompileVertex(x, y);
        m_triangles += 4;
        return;
    }

    // 如果顶部边缘点不活跃

    if (!POINT(xc, y)) // 顶部边缘不活跃
    {
        if (POINT(x, yc) && POINT(x2, yc))
        { // 左侧和右侧边缘活跃
            CompileFan(xc, yc, x, yc, x, y, x2, y, x2, yc);
        }
        else
        {  // 左侧或右侧边缘不活跃
            CompileVertex(xc, yc);

            if (POINT(x2, yc))
            { // EL
                CompileVertex(x2, yc);
                m_triangles++;
            }

            CompileVertex(x2, y);
            CompileVertex(x, y);

            if (POINT(x, yc))
            {
                CompileVertex(x, yc); // WR
                m_triangles++;
            }
            m_triangles++;
        }
    }

    // 如果底部边缘点不活跃
    if (!POINT(xc, y2)
    { // 底部边缘不活跃
        if (POINT(x, yc) && POINT(x2, yc))
        { // 左侧和右侧边缘活跃
            CompileFan(xc, yc, x2, yc, x2, y2, x, y2, x, yc);
        }
        else
        {
            CompileVertex(xc, yc);

            if (POINT(x, yc))
            {
                CompileVertex(x, yc);
                m_triangles++;
            }

            CompileVertex(x, y2);
            CompileVertex(x2, y2);

            if (POINT(x2, yc))
            {
                CompileVertex(x2, yc);
                m_triangles++;
            }

            m_triangles++;
        }
    }

    // 如果左侧边缘点不活跃
    if (!POINT(x, yc))
    {
        // 左侧边缘不活跃
        if (POINT(xc, y) && POINT(xc, y2))
        { // 顶部和底部边缘活跃
            CompileFan(xc, yc, xc, y2, x, y2, x, y, xc, y);
        }
        else
        {
            CompileVertex(xc, yc);

            if (POINT(xc, y))
            { // NL
                CompileVertex(xc, y);
                m_triangles++;
            }

            CompileVertex(x, y);
            CompileVertex(x, y2);

            if (POINT(xc, y2))
            {
                CompileVertex(xc, y2); // SR
                m_triangles++;
            }

            m_triangles++;
        }
    }

    // 如果右侧边缘点不活跃
    if (!POINT(x2, yc))
    { // 右侧边缘不活跃
        if (POINT(xc, y) && POINT(xc, y2))
        {
            CompileFan(xc, yc, xc, y, x2, y, x2, y2, xc, y2);
        }
        else
        {
            CompileVertex(xc, yc);
            if (POINT(xc, y2))
            { // SL
                CompileVertex(xc, y2);
                m_triangles++;
            }
            CompileVertex(x2, y2);
            CompileVertex(x2, y);
            if (POINT(xc, y))
            {
                CompileVertex(xc, y); // NR
                m_triangles++;
            }
            m_triangles++;
        }
    }

    // 递归添加子块
    if (POINT(xc, y) && POINT(x, yc))
        CompileBlock(x, y, next_size); // 子块 A
    if (POINT(xc, y) && POINT(x2, yc))
        CompileBlock(x + next_size, y, next_size); // 子块 B
    if (POINT(x, yc) && POINT(xc, y2))
        CompileBlock(x, y + next_size, next_size); // 子块 C
    if (POINT(x2, yc) && POINT(xc, y2))
        CompileBlock(x + next_size, y + next_size, next_size); // 子块 D
}

void Terrain::CompileTriangle(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t x3, uint32_t y3)
{
    // 添加顶点并获取它们的索引
    uint32_t v0 = CompileVertex(x3, y3);
    uint32_t v1 = CompileVertex(x2, y2);
    uint32_t v2 = CompileVertex(x1, y1);

    // 添加索引以形成一个三角形
    indices.push_back(v0);
    indices.push_back(v1);
    indices.push_back(v2);

    // 更新三角形计数
    m_triangles++;
}

void Terrain::Compile()
{
    unsigned long compile_start;
    uint32_t x, y;

    //    compile_start = GetTickCount();

        // 如果是第一个区域，重置计数器和数据容器
    if (m_zone == 0) {
        m_vertices = 0;
        m_triangles = 0;
        //       m_compile_time = 0;
        vertices.clear();
        indices.clear();
    }

    // 计算当前区域的x和y坐标
    x = m_zone % ZONE_GRID;
    y = (m_zone - x) / ZONE_GRID;
    m_zone_origin_x = x * m_zone_size;
    m_zone_origin_y = y * m_zone_size;

    // 绑定纹理并设置纹理参数（假设每个区域有不同的纹理）
    GLuint texture = MapTexture(m_zone);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    // 编译固体部分
    if (DO_SOLID)
    {
        // 设置多边形模式为填充
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        // 启用面剔除
        glEnable(GL_CULL_FACE);
        // 禁用混合和颜色材质
        glDisable(GL_BLEND);
        glDisable(GL_COLOR_MATERIAL);
        // 设置颜色为白色（应通过着色器统一变量管理颜色）
        // glColor3f (1.0f, 1.0f, 1.0f); // 已移除
        m_use_color = false;

        // 编译区域块，将顶点和索引添加到容器中
        CompileBlock(x * m_zone_size, y * m_zone_size, m_zone_size);
    }

    // 编译线框部分
    if (DO_WIREFRAME)
    {
        // 禁用面剔除，启用线平滑和混合
        glDisable(GL_CULL_FACE);
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
        // 设置线框颜色（应通过着色器统一变量管理颜色）
        // glColor4f (0.4f, 0.4f, 1.0f, 0.1f); // 已移除
        // 设置多边形模式为线框
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(2.0f);
        // 启用颜色材质（应通过着色器统一变量管理颜色）
        glEnable(GL_COLOR_MATERIAL);

        // 编译区域块，将线框顶点和索引添加到容器中
        CompileBlock(x * m_zone_size, y * m_zone_size, m_zone_size);

        // 重置混合函数和多边形模式
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    m_zone++;

    // 如果所有区域已编译
    if (m_zone == ZONES)
    {
        if (DO_WIREFRAME && DO_SOLID) {
            // 如果同时编译了固体和线框，调整顶点和三角形计数
            m_vertices /= 2;
            m_triangles /= 2;
        }
        m_zone = 0;
        m_stage++;
        //Console ("Compiled %d polys, %d verts in %dms", m_triangles, m_vertices, m_compile_time);
    }

    // 记录编译时间
 //   m_compile_time += GetTickCount() - compile_start;

    // 当所有区域编译完成后，上传数据到 GPU
    if (m_zone == 0 && m_stage >= some_threshold)
    {
        // 需要根据实际逻辑调整
        UploadData();
    }
}

// 上传数据到GPU
void Terrain::UploadData()
{
    glBindVertexArray(VAO);

    // 上传顶点数据
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

    // 上传索引数据
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(unsigned int), indices.data());

    glBindVertexArray(0);
}
/*
// 渲染函数
// 渲染地形
    void Render(GLuint shaderProgram)
    {
        // 使用着色器程序
        glUseProgram(shaderProgram);

        // 绑定 VAO
        glBindVertexArray(VAO);

        // 绘制固体部分
        if (DO_SOLID) {
            // 设置颜色统一变量
            glUniform4f(glGetUniformLocation(shaderProgram, "objectColor"), 1.0f, 1.0f, 1.0f, 1.0f);
            // 绘制三角形
            glDrawElements(GL_TRIANGLES, m_triangles * 3, GL_UNSIGNED_INT, 0);
        }

        // 绘制线框部分
        if (DO_WIREFRAME) {
            // 设置颜色统一变量
            glUniform4f(glGetUniformLocation(shaderProgram, "objectColor"), 0.4f, 0.4f, 1.0f, 0.1f);
            // 设置多边形模式为线框
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            // 绘制三角形
            glDrawElements(GL_TRIANGLES, m_triangles * 3, GL_UNSIGNED_INT, 0);
            // 重置多边形模式为填充
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        // 解绑 VAO
        glBindVertexArray(0);
    }
};
*/