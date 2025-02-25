﻿/**************************************************************************************************************************
Copyright(C) 2014-2017 www.xionggf.com

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
#include "font_app.h"
#include "../klib/kgl_defines.h"
#include "../klib/kgl_string_convertor.h"
#include "../klib/kgl_font_renderer.h"

FontApp::FontApp()
{
}

FontApp::~FontApp()
{
    
}

void FontApp::PreRenderFrame()
{
    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FontApp::InitScene()
{
    kgl::FontRenderer* font_renderer = kgl::KFontRenderer::GetInstance();
    font_renderer->Initialize();
    font_renderer->CreateFontTexture("resources/font/fzss_gbk.ttf", "fzss64", 64, 2048, 2048);
    font_renderer->CreateFontTexture("resources/font/wqy_wmh.ttf", "wqy_wmh64", 64, 2048, 2048);
    font_renderer->CreateFontTexture("resources/font/fzkt_sim.ttf", "fzkt64", 64, 2048, 2048);
    font_renderer->CreateFontTexture("resources/font/meiryo.ttc", "meiryo64", 64, 2048, 2048);
    font_renderer->CreateFontTexture("resources/font/mao.ttf", "mao64", 48, 4096, 4096);

    text_1_ = kgl::StringConvertor::UTF8toUTF16LE("方正简宋： KumaGL图形学学习框架");
    text_2_ = kgl::StringConvertor::UTF8toUTF16LE("文泉驿微米黑 开彼源兮 斯流永继");
    text_3_ = kgl::StringConvertor::UTF8toUTF16LE("方正楷体 兰亭序");
    text_jpn_ = kgl::StringConvertor::UTF8toUTF16LE("Meiryo あいうえおオハヨウｵﾊﾖｳ働峠颪畠辻");

    text_long_march_title_ = kgl::StringConvertor::UTF8toUTF16LE("         七律 长征 毛泽东");
    text_long_march_3_ = kgl::StringConvertor::UTF8toUTF16LE("金沙水拍云崖暖 大渡桥横铁索寒"); // 
    text_long_march_4_ = kgl::StringConvertor::UTF8toUTF16LE("更喜岷山千里雪 三军过后尽开颜");
    text_long_march_1_ = kgl::StringConvertor::UTF8toUTF16LE("红军不怕远征难 万水千山只等闲");
    text_long_march_2_ = kgl::StringConvertor::UTF8toUTF16LE("五岭逶迤腾细浪 乌蒙磅礴走泥丸");
}

void FontApp::InitHelper()
{
    GLfloat vertices[] =
    {
        // 位置               // 纹理映射坐标
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // 右上角
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // 右下角
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // 左下角
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f  //左上角
    };

    GLuint indices[] =
    {
        0, 1, 3,
        1, 2, 3
    };

    kgl::VertexAttribute va_position;
    va_position.index = 0;
    va_position.normalized = GL_FALSE;
    va_position.type = GL_FLOAT;
    va_position.size = 3; // 一个“顶点位置”的属性由3个分量组成
    va_position.stride = 5 * sizeof(GLfloat); // 每个顶点的步长
    va_position.pointer = nullptr;

    kgl::VertexAttribute va_texture_coord;
    va_texture_coord.index = 1;
    va_texture_coord.normalized = GL_FALSE;
    va_texture_coord.type = GL_FLOAT;
    va_texture_coord.size = 2; // 一个“顶点坐标”的属性由3个分量组成
    va_texture_coord.stride = 5 * sizeof(GLfloat); // 每个顶点的步长
    va_texture_coord.pointer = reinterpret_cast<GLvoid*> (3 * sizeof(GLfloat)); // 纹理坐标数据在字段中的起始指针处

    std::vector<kgl::VertexAttribute> vtx_attri_array;
    vtx_attri_array.push_back(va_position);
    vtx_attri_array.push_back(va_texture_coord);

    helper_rectangle_ = new kgl::Primitive;
    helper_rectangle_->CreateIndexed(GL_TRIANGLES, vertices, sizeof(vertices), GL_STATIC_DRAW, kgl::Primitive::UINT32, indices, sizeof(indices), GL_STATIC_DRAW, vtx_attri_array);
}

void FontApp::RenderScene()
{
    kgl::FontRenderer* font_renderer = kgl::KFontRenderer::GetInstance();
    font_renderer->SetCurrentFont("fzss64");
    
    font_renderer->AddToRendered(text_1_, 10, 0,
        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 
        glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
        glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), 1.f);

    font_renderer->SetCurrentFont("wqy_wmh64");
    font_renderer->AddToRendered(text_2_, 10, 80, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), 1.f);

    font_renderer->SetCurrentFont("fzkt64");
    font_renderer->AddToRendered(text_3_, 10, 150, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 1.f);

    font_renderer->SetCurrentFont("meiryo64");
    font_renderer->AddToRendered(text_jpn_, 10, 220, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 1.f);

    font_renderer->SetCurrentFont("mao64");
    font_renderer->AddToRendered(text_long_march_title_, 10, 300, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.f);
    font_renderer->AddToRendered(text_long_march_1_, 10, 360, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.f);
    font_renderer->AddToRendered(text_long_march_2_, 10, 420, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.f);
    font_renderer->AddToRendered(text_long_march_3_, 10, 480, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.f);
    font_renderer->AddToRendered(text_long_march_4_, 10, 540, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.f);

    font_renderer->Draw();
}