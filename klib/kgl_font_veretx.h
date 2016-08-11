// 字体顶点
#ifndef kgl_font_veretx_h__
#define kgl_font_veretx_h__

namespace kgl
{
	struct FontVertex   // 顶点结构描述
	{
		FontVertex();
		FontVertex(float x, float y, float r, float g, float b, float _a, float u, float v);
		void Reset();
		float PosX, PosY; // 基于NDC坐标
		float R, G, B, A;
		float U, V; // 纹理的uv坐标
	};
}

#endif // kgl_font_veretx_h__
